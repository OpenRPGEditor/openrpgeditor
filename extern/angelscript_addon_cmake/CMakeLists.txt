add_library(angelscript_addons STATIC
        ../angelscript/add_on/contextmgr/contextmgr.h
        ../angelscript/add_on/contextmgr/contextmgr.cpp
        ../angelscript/add_on/datetime/datetime.h
        ../angelscript/add_on/datetime/datetime.cpp
        ../angelscript/add_on/debugger/debugger.h
        ../angelscript/add_on/debugger/debugger.cpp
        ../angelscript/add_on/scriptany/scriptany.h
        ../angelscript/add_on/scriptany/scriptany.cpp
        ../angelscript/add_on/scriptarray/scriptarray.h
        ../angelscript/add_on/scriptarray/scriptarray.cpp
        ../angelscript/add_on/scriptbuilder/scriptbuilder.cpp
        ../angelscript/add_on/scriptbuilder/scriptbuilder.h
        ../angelscript/add_on/scriptdictionary/scriptdictionary.cpp
        ../angelscript/add_on/scriptdictionary/scriptdictionary.h
        ../angelscript/add_on/scriptfile/scriptfile.cpp
        ../angelscript/add_on/scriptfile/scriptfile.h
        ../angelscript/add_on/scriptgrid/scriptgrid.cpp
        ../angelscript/add_on/scriptgrid/scriptgrid.h
        ../angelscript/add_on/scripthandle/scripthandle.cpp
        ../angelscript/add_on/scripthandle/scripthandle.h
        ../angelscript/add_on/scriptmath/scriptmath.h
        ../angelscript/add_on/scriptmath/scriptmath.cpp
        ../angelscript/add_on/scriptmath/scriptmathcomplex.h
        ../angelscript/add_on/scriptmath/scriptmathcomplex.cpp
        ../angelscript/add_on/scriptstdstring/scriptstdstring.h
        ../angelscript/add_on/scriptstdstring/scriptstdstring.cpp
        ../angelscript/add_on/scriptstdstring/scriptstdstring_utils.cpp
        ../angelscript/add_on/scriptjson/scriptjson.h
        ../angelscript/add_on/scriptjson/scriptjson.cpp
        ../angelscript/add_on/serializer/serializer.h
        ../angelscript/add_on/serializer/serializer.cpp
        ../angelscript/add_on/weakref/weakref.cpp
        ../angelscript/add_on/weakref/weakref.h
)

target_include_directories(
        angelscript_addons PUBLIC
        ../angelscript/angelscript/include
        ../angelscript/add_on
)

target_link_libraries(
        angelscript_addons PUBLIC
        angelscript
)