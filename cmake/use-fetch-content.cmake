cmake_minimum_required(VERSION 3.24)

include(FetchContent)

if(NOT BEMAN_INFRA_LOCKFILE)
    set(BEMAN_INFRA_LOCKFILE
        "lockfile.json"
        CACHE FILEPATH
        "Path to the dependency lockfile for the Beman Infra provider."
    )
endif()

set(BemanInfra_projectDir "${CMAKE_CURRENT_LIST_DIR}/../")
message(TRACE "BemanInfra_projectDir=\"${BemanInfra_projectDir}\"")

message(TRACE "BEMAN_INFRA_LOCKFILE=\"${BEMAN_INFRA_LOCKFILE}\"")
file(
    REAL_PATH "${BEMAN_INFRA_LOCKFILE}"
    BemanInfra_lockfile
    BASE_DIRECTORY "${BemanInfra_projectDir}"
    EXPAND_TILDE
)
message(DEBUG "Using lockfile: \"${BemanInfra_lockfile}\"")

# Force CMake to reconfigure the project if the lockfile changes
set_property(
    DIRECTORY "${BemanInfra_projectDir}"
    APPEND
    PROPERTY CMAKE_CONFIGURE_DEPENDS "${BemanInfra_lockfile}"
)

# For more on the protocol for this function, see:
# https://cmake.org/cmake/help/latest/command/cmake_language.html#provider-commands
function(BemanInfra_provideDependency method package_name)
    # Read the lockfile
    file(READ "${BemanInfra_lockfile}" BemanInfra_rootObj)

    # Get the "dependencies" field and store it in BemanInfra_dependenciesObj
    string(
        JSON BemanInfra_dependenciesObj
        ERROR_VARIABLE BemanInfra_error
        GET "${BemanInfra_rootObj}"
        "dependencies"
    )
    if(BemanInfra_error)
        message(FATAL_ERROR "${BemanInfra_lockfile}: ${BemanInfra_error}")
    endif()

    # Get the length of the libraries array and store it in BemanInfra_dependenciesObj
    string(
        JSON BemanInfra_numDependencies
        ERROR_VARIABLE BemanInfra_error
        LENGTH "${BemanInfra_dependenciesObj}"
    )
    if(BemanInfra_error)
        message(FATAL_ERROR "${BemanInfra_lockfile}: ${BemanInfra_error}")
    endif()

    if(BemanInfra_numDependencies EQUAL 0)
        return()
    endif()

    # Loop over each dependency object
    math(EXPR BemanInfra_maxIndex "${BemanInfra_numDependencies} - 1")
    foreach(BemanInfra_index RANGE "${BemanInfra_maxIndex}")
        set(BemanInfra_errorPrefix
            "${BemanInfra_lockfile}, dependency ${BemanInfra_index}"
        )

        # Get the dependency object at BemanInfra_index
        # and store it in BemanInfra_depObj
        string(
            JSON BemanInfra_depObj
            ERROR_VARIABLE BemanInfra_error
            GET "${BemanInfra_dependenciesObj}"
            "${BemanInfra_index}"
        )
        if(BemanInfra_error)
            message(
                FATAL_ERROR
                "${BemanInfra_errorPrefix}: ${BemanInfra_error}"
            )
        endif()

        # Get the "name" field and store it in BemanInfra_name
        string(
            JSON BemanInfra_name
            ERROR_VARIABLE BemanInfra_error
            GET "${BemanInfra_depObj}"
            "name"
        )
        if(BemanInfra_error)
            message(
                FATAL_ERROR
                "${BemanInfra_errorPrefix}: ${BemanInfra_error}"
            )
        endif()

        # Get the "package_name" field and store it in BemanInfra_pkgName
        string(
            JSON BemanInfra_pkgName
            ERROR_VARIABLE BemanInfra_error
            GET "${BemanInfra_depObj}"
            "package_name"
        )
        if(BemanInfra_error)
            message(
                FATAL_ERROR
                "${BemanInfra_errorPrefix}: ${BemanInfra_error}"
            )
        endif()

        # Get the "git_repository" field and store it in BemanInfra_repo
        if(DEFINED "BEMANINFRA_${BemanInfra_name}_REPO")
            set(BemanInfra_repo ${BEMANINFRA_${BemanInfra_name}_REPO})
        else()
            string(
                JSON BemanInfra_repo
                ERROR_VARIABLE BemanInfra_error
                GET "${BemanInfra_depObj}"
                "git_repository"
            )
            if(BemanInfra_error)
                message(
                    FATAL_ERROR
                    "${BemanInfra_errorPrefix}: ${BemanInfra_error}"
                )
            endif()
        endif()

        # Get the "git_tag" field and store it in BemanInfra_tag
        string(
            JSON BemanInfra_tag
            ERROR_VARIABLE BemanInfra_error
            GET "${BemanInfra_depObj}"
            "git_tag"
        )
        if(BemanInfra_error)
            message(
                FATAL_ERROR
                "${BemanInfra_errorPrefix}: ${BemanInfra_error}"
            )
        endif()

        if(method STREQUAL "FIND_PACKAGE")
            if(package_name STREQUAL BemanInfra_pkgName)
                string(
                    APPEND BemanInfra_debug
                    "Redirecting find_package calls for ${BemanInfra_pkgName} "
                    "to FetchContent logic.\n"
                    string
                    APPEND BemanInfra_debug
                    "Fetching ${BemanInfra_repo} at "
                    "${BemanInfra_tag} according to ${BemanInfra_lockfile}."
                )
                message(DEBUG "${BemanInfra_debug}")
                FetchContent_Declare(
                    "${BemanInfra_name}"
                    GIT_REPOSITORY "${BemanInfra_repo}"
                    GIT_TAG "${BemanInfra_tag}"
                    EXCLUDE_FROM_ALL
                )
                FetchContent_MakeAvailable("${BemanInfra_name}")

                # Important! <PackageName>_FOUND tells CMake that `find_package` is
                # not needed for this package anymore
                set("${BemanInfra_pkgName}_FOUND" TRUE PARENT_SCOPE)
            endif()
        endif()
    endforeach()
endfunction()

cmake_language(
    SET_DEPENDENCY_PROVIDER BemanInfra_provideDependency
    SUPPORTED_METHODS FIND_PACKAGE
)

# Add this dir to the module path so that `find_package(beman-install-library)` works
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}")
