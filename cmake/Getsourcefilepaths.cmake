function(get_target_sources_with_paths target sources_with_paths)
    get_target_property(sources ${target} SOURCES)
    get_target_property(source_dir ${target} SOURCE_DIR)

    foreach (source ${sources})
        # Check if the source file is a generator expression
        if (NOT "${source}" MATCHES "^<")
            # Construct the full path to the source file
            get_filename_component(abs_source_path "${source_dir}/${source}" ABSOLUTE)
            file(RELATIVE_PATH source_path ${CMAKE_SOURCE_DIR} ${abs_source_path})
            list(APPEND ${sources_with_paths} "${source_path}")
        else ()
            # Append generator expressions as they are
            list(APPEND ${sources_with_paths} "${source}")
        endif ()
    endforeach ()

    set(${sources_with_paths} ${${sources_with_paths}} PARENT_SCOPE)
endfunction()