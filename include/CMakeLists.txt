project(Include)

set(HEADER_FILES
        core/Date.h
        core/SimpleString.h
        core/SimpleTime.h
        core/Library.h

        model/Book.h
        model/BookInstance.h
        model/Order.h
        model/User.h

        util/DbAdapter.h
        util/TableRenderer.h
        util/Bookcopy.h
        util/Record.h

        service/BookService.h
        service/UserService.h


        )

set(SOURCE_FILES
        ../src/core/Date.cpp
        ../src/core/SimpleString.cpp
        ../src/core/SimpleTime.cpp
        ../src/core/Library.cpp

        ../src/model/Book.cpp
        ../src/model/BookInstance.cpp
        ../src/model/User.cpp
        ../src/model/Order.cpp

        ../src/util/DbAdapter.cpp
        ../src/util/TableRenderer.cpp

        ../src/service/BookService.cpp
        ../src/service/UserService.cpp
        )
add_library(Include STATIC ${SOURCE_FILES} ${HEADER_FILES})