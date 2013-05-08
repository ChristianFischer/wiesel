/**
 * Copyright (C) 2012
 * Christian Fischer
 *
 * https://bitbucket.org/baldur/wiesel/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 */
#ifndef __WIESEL_IO_NET_MESSAGEDISPATCHER_H__
#define	__WIESEL_IO_NET_MESSAGEDISPATCHER_H__

#include <wiesel/io/uri.h>
#include <wiesel/util/shared_object.h>
#include <wiesel/wiesel-net.def>

#include <wiesel/util/thread.h>

#include "connection.h"

#include <algorithm>


namespace wiesel {

	/**
	 * @brief Baseclass for an object which handles a network connection
	 * to receive custom formatted messages.
	 * Classes implementing a message dispatcher should not extend from this class,
	 * but use \ref TMessageDispatcher.
	 */
	class WIESEL_NET_EXPORT MessageDispatcher :
			public virtual SharedObject,
			public ListenerSupport<ConnectionListener>,
			protected IRunnable
	{
	public:
		/**
		 * @brief Baseclass for any messages handled by \ref MessageDispatcher classes.
		 */
		class WIESEL_NET_EXPORT IMessage : public virtual SharedObject
		{
		protected:
			IMessage();

		public:
			virtual ~IMessage();
		};

		/**
		 * @brief A list of flags to control the dispatcher's behaviour.
		 */
		enum DispatchFlag {
			/**
			 * @brief Automatically tries to restore the connection when disconnected.
			 */
			AutoReconnect			= 0x01,
		};

	protected:
		MessageDispatcher();

	public:
		virtual ~MessageDispatcher();

	// flags
	public:
		/**
		 * @brief Adds a flag to this dispatcher.
		 */
		void addFlag(DispatchFlag flag);

		/**
		 * @brief Removes a flag from this dispatcher.
		 */
		void removeFlag(DispatchFlag flag);

		/**
		 * @brief Checks if a specific flag is set for this dispatcher.
		 */
		bool hasFlag(DispatchFlag flag) const;

	public:
		/**
		 * @brief Tries to connect to the given address and starts reading message from the connection.
		 */
		virtual void start(const std::string& address);

		/**
		 * @brief Tries to connect to the given URI and starts reading message from the connection.
		 */
		virtual void start(const URI& uri);

		/**
		 * @brief Stops listening for messages and closes the connection.
		 */
		virtual void stop();

		/**
		 * @brief Dispatches all cached messages to the registered message receiver.
		 * This should be called in your mainthread.
		 */
		virtual void dispatch() = 0;

	// getter
	public:
		/**
		 * @brief Get the currently active connection, if any.
		 */
		inline Connection* getConnection() {
			return connection;
		}

	// thread support for subclasses
	protected:
		/**
		 * @brief Get the current thread-object.
		 * Should be valid until the dispatcher has been stopped.
		 */
		inline Thread* getCurrentThread() {
			return thread;
		}

		/**
		 * @brief Locks the current thread.
		 */
		void lock();

		/**
		 * @brief Unlocks the current thread.
		 */
		void unlock();

	// overridables
	protected:
		/**
		 * @brief To be overridden by subclasses to load a new message from the current connection.
		 */
		virtual bool fetchMessage(Connection *connection) = 0;

	// IRunnable
	protected:
		virtual void run();

	private:
		URI				uri;
		Thread*			thread;
		Connection*		connection;
		uint32_t		flags;
		bool			stopped;
	};



	/**
	 * @brief Baseclass for custom message dispatcher implementations.
	 */
	template<class MESSAGE_TYPE>
	class TMessageDispatcher : public MessageDispatcher
	{
	public:
		/**
		 * @brief The listener class which will receive network messages.
		 */
		class MessageReceiver : public virtual SharedObject
		{
		public:
			MessageReceiver() { }
			virtual ~MessageReceiver() { }

		public:
			virtual void onMessageReceived(MESSAGE_TYPE* message) = 0;
		};

	public:
		typedef typename std::list<MessageReceiver*>	ReceiverList;
		typedef typename ref<MESSAGE_TYPE>::list		MessageList;

	protected:
		TMessageDispatcher() {
			return;
		}

	public:
		virtual ~TMessageDispatcher() {
			return;
		}

	public:
		/**
		 * @brief Adds a new receiver to receive messages.
		 */
		void addReceiver(MessageReceiver *receiver) {
			this->lock();
			if (std::find(receivers.begin(), receivers.end(), receiver) == receivers.end()) {
				receivers.push_back(receiver);
			}
			this->unlock();
		}

		/**
		 * @brief Removes a receiver from this class.
		 */
		void removeReceiver(MessageReceiver *receiver) {
			this->lock();

			typename std::list<MessageReceiver>::iterator it;
			it = std::find(receivers.begin(), receivers.end(), receiver);

			if (it != receivers.end()) {
				receivers.erase(it);
			}

			this->unlock();
		}

	public:
		virtual void dispatch() {
			this->lock();

			// doesn't make sense to call dispatch without having receivers attached?
			assert(receivers.empty() == false);

			for(typename MessageList::iterator msg_it=messages.begin(); msg_it!=messages.end();) {
				MESSAGE_TYPE *message = *msg_it;

				for(typename ReceiverList::iterator rcv_it=receivers.begin(); rcv_it!=receivers.end(); rcv_it++) {
					(*rcv_it)->onMessageReceived(message);
				}

				msg_it = messages.erase(msg_it);
			}

			this->unlock();
		}

	protected:
		/**
		 * @brief Reads a new message from the connection.
		 * To be implemented by subclasses.
		 */
		virtual MESSAGE_TYPE* readMessage(Connection *connection) = 0;

	private:
		virtual bool fetchMessage(Connection *connection) {
			MESSAGE_TYPE* message = readMessage(connection);
			if (message) {
				this->lock();
				messages.push_back(message);
				this->unlock();

				return true;
			}

			return false;
		}

	private:
		ReceiverList	receivers;
		MessageList		messages;
	};

}

#endif // __WIESEL_IO_NET_MESSAGEDISPATCHER_H__

