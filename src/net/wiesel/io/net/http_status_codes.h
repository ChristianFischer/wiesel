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
#ifndef __WIESEL_IO_NET_HTTP_STATUS_CODES_H__
#define __WIESEL_IO_NET_HTTP_STATUS_CODES_H__

namespace wiesel {

	/**
	 * @brief A list of possible HTTP-Status codes
	 */
	enum HttpStatusCode {
		HTTP_Continue						= 100,
		HTTP_SwitchingProtocols				= 101,
		HTTP_Processing						= 102,

		HTTP_OK								= 200,
		HTTP_Created						= 201,
		HTTP_Accepted						= 202,
		HTTP_NonAuthorativeInformation		= 203,
		HTTP_NoContent						= 204,
		HTTP_ResetContent					= 205,
		HTTP_PartialContent					= 206,
		HTTP_MultiStatus					= 207,

		HTTP_MultipleChoices				= 300,
		HTTP_MovedPermanently				= 301,
		HTTP_Found							= 302,
		HTTP_SeeOther						= 303,
		HTTP_NotModified					= 304,
		HTTP_UseProxy						= 305,
		HTTP_TemporaryRedirect				= 307,

		HTTP_BadRequest						= 400,
		HTTP_Unauthorized					= 401,
		HTTP_PaymentRequired				= 402,
		HTTP_Forbidden						= 403,
		HTTP_NotFound						= 404,
		HTTP_MethodNotAllowed				= 405,
		HTTP_NotAcceptable					= 406,
		HTTP_ProxyAuthenticationRequired	= 407,
		HTTP_RequestTimeout					= 408,
		HTTP_Conflict						= 409,
		HTTP_Gone							= 410,
		HTTP_LengthRequired					= 411,
		HTTP_PreconditionFailed				= 412,
		HTTP_RequestEntitiyTooLarge			= 413,
		HTTP_RequestUrlTooLong				= 414,
		HTTP_UnsupportedMediaType			= 415,
		HTTP_RequestedRangeNotSatisfiable	= 416,
		HTTP_ExpectationFailed				= 417,
		HTTP_ImATeapot						= 418,
		HTTP_PolicyNotFulfilled				= 420,
		HTTP_TooManyConnections				= 421,
		HTTP_UnprocessableEntity			= 422,
		HTTP_Locked							= 423,
		HTTP_FailedDependency				= 424,
		HTTP_UnorderedCollection			= 425,
		HTTP_UpgradeRequired				= 426,
		HTTP_TooManyRequests				= 429,

		HTTP_InternalServerError			= 500,
		HTTP_NotImplemented					= 501,
		HTTP_BadGateway						= 502,
		HTTP_ServiceUnavailable				= 503,
		HTTP_GatewayTimeout					= 504,
		HTTP_HttpVersionNotSupported		= 505,
		HTTP_VariantAlsoNegotiates			= 506,
		HTTP_InsufficientStorage			= 507,
		HTTP_BandwidthLimitExceeded			= 509,
		HTTP_NotExtended					= 510,
	};

}

#endif // __WIESEL_IO_NET_HTTP_STATUS_CODES_H__

