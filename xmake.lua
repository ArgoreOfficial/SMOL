
set_symbols("debug")
set_optimize("none")

add_includedirs( "src/auxiliary" );
add_includedirs( "src/common" );

target( "common" )
    set_kind( "static" )
    add_headerfiles( 
        "src/common/**.h",
        "src/common/**.hpp" 
        )
    add_files( "src/common/**.cpp" )
target_end()

target( "cpatcher" )
    set_kind( "binary" )
    add_deps( "common" )
    set_basename("cpatcher_$(mode)_$(arch)")

    set_targetdir "bin"
    set_objectdir "build/obj"

    add_headerfiles( 
        "src/cpatcher/**.h",
        "src/cpatcher/**.hpp" 
        )
    add_files( "src/cpatcher/**.cpp" )
target_end()