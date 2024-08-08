
set_symbols("debug")
set_optimize("none")

add_includedirs( "src/auxiliary" );

target( "cpatcher" )
    set_kind( "binary" )
    set_basename("cpatcher_$(mode)_$(arch)")

    set_targetdir "bin"
    set_objectdir "build/obj"

    add_headerfiles( 
        "src/cpatcher/**.h",
        "src/cpatcher/**.hpp" 
        )
    add_files( "src/cpatcher/**.cpp" )
target_end()