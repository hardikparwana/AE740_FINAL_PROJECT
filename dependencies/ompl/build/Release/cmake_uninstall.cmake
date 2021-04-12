if (NOT EXISTS "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/install_manifest.txt\"")
endif(NOT EXISTS "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/install_manifest.txt")

file(READ "/media/hardik/Windows/Ubuntu/catkin_740/src/AE740_FINAL_PROJECT/dependencies/ompl/build/Release/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
list(REVERSE files)
foreach (file ${files})
    message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    if (EXISTS "$ENV{DESTDIR}${file}")
        execute_process(
            COMMAND /usr/bin/cmake -E remove "$ENV{DESTDIR}${file}"
            OUTPUT_VARIABLE rm_out
            RESULT_VARIABLE rm_retval
        )
        if(NOT ${rm_retval} EQUAL 0)
            message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
        endif (NOT ${rm_retval} EQUAL 0)
    else (EXISTS "$ENV{DESTDIR}${file}")
        message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
    endif (EXISTS "$ENV{DESTDIR}${file}")
endforeach(file)
