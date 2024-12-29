# Assets for all platforms
set(SHARED_STATIC_ASSETS ${PROJECT_SOURCE_DIR}/src/assets/fonts/MPLUSRounded1c-Medium.ttf)
set(SHARED_STATIC_ASSETS ${PROJECT_SOURCE_DIR}/src/assets/fonts/NotoSansSinhala-Medium.ttf)
set(SHARED_STATIC_ASSETS ${PROJECT_SOURCE_DIR}/src/assets/fonts/JetBrainsMono-Medium.ttf)
set(SHARED_STATIC_ASSETS ${PROJECT_SOURCE_DIR}/src/assets/fonts/mplus-1m-regular.ttf)
set(SHARED_STATIC_ASSETS ${PROJECT_SOURCE_DIR}/src/assets/images/tilemarkers_64x80.png)

# Platform specific static assets
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(${NAME} PUBLIC
            ${SHARED_STATIC_ASSETS}
            ${PROJECT_SOURCE_DIR}/src/assets/icons/icon.ico
            ${PROJECT_SOURCE_DIR}/src/openrpgeditor/Manifests/app.rc
            ${PROJECT_SOURCE_DIR}/src/openrpgeditor/Manifests/App.manifest)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(MACOSX_STATIC_ASSETS
            ${SHARED_STATIC_ASSETS}
            ${PROJECT_SOURCE_DIR}/src/assets/icons/icon.icns)
    target_sources(${NAME} PUBLIC ${MACOSX_STATIC_ASSETS})
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(${NAME} PRIVATE ${SHARED_STATIC_ASSETS})
endif ()
