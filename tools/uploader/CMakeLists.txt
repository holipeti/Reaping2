
include_directories( SYSTEM ${CURL_INCLUDE_DIR} )
add_executable( r2uploader uploader.cpp )
add_dependencies( r2uploader version )
target_link_libraries( r2uploader
    ${CURL_LIBRARY} )

