project(Include)

set(HEADER_FILES
        core/Date.h
        core/SimpleString.h
        core/SimpleTime.h
        )

set(SOURCE_FILES
        ../src/core/Date.cpp
        ../src/core/SimpleString.cpp
        ../src/core/SimpleTime.cpp
        )
add_library(Include STATIC ${SOURCE_FILES} ${HEADER_FILES})