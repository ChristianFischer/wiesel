# wiesel-directx11 library


# create the DirectX11 module
wiesel_create_module(wiesel-directx11 ${WIESEL_SRC_DIR}/directx11 ${WIESEL_TESTS_DIR}/directx11)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-directx11 ${WIESEL_SRC_DIR}/directx11)

# add required modules
wiesel_module_add_dependency(wiesel-directx11 wiesel-base)
wiesel_module_add_dependency(wiesel-directx11 wiesel-core)

include(wiesel-FindDirectX)

if (DIRECTX_D3D11_FOUND)
	wiesel_target_add_includes(wiesel-directx11 ${DIRECTX_D3D11_INCLUDE_DIR})
	wiesel_target_add_libraries(wiesel-directx11 ${DIRECTX_D3D11_LIBRARY} dxgi d3dcompiler)
	
	if (DEFINED WIN8_SDK_INCLUDE_DIR)
		wiesel_target_add_includes(wiesel-directx11 "${WIN8_SDK_INCLUDE_DIR}")
	endif(DEFINED WIN8_SDK_INCLUDE_DIR)
else(DIRECTX_D3D11_FOUND)
	message(FATAL_ERROR "DirectX 11 libraries not found!")
endif(DIRECTX_D3D11_FOUND)
