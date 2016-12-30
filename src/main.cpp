#include <windows.h>
#include <direct.h>
#include <iostream>

std::string currentDirName() {
    char buffer[FILENAME_MAX];

    if (!_getcwd(buffer, sizeof(buffer)))
        return "";

    std::string str(buffer);
    std::string::size_type pos = str.find_last_of("\\") + 1;

    return str.substr(pos);
}

static const char *gitignore =
    "bin/\n"
    "CMakeFiles/\n"
    "cmake_install.cmake\n"
    "CmakeCache.txt\n"
    "*.sublime-workspace\n"
    "Makefile\n";

static const char *cmakeLists =
    "cmake_minimum_required(VERSION 3.7.0)\n"
    "\n"
    "set(CMAKE_COLOR_MAKEFILE off)\n"
    "set(CMAKE_CXX_STANDARD 11)\n"
    "\n"
    "project(%s)\n"
    "\n"
    "file(GLOB SOURCES \"src/*.cpp\")\n"
    "\n"
    "set(EXECUTABLE_OUTPUT_PATH \"bin\")\n"
    "add_executable(%s ${SOURCES})\n";

static const char *project =
    "{\n"
    "    \"folders\":\n"
    "    [\n"
    "        {\n"
    "            \"path\": \"./\",\n"
    "            \"folder_exclude_patterns\": [\"bin\", \"CMakeFiles\"],\n"
    "            \"file_exclude_patterns\": [\"cmake_install.cmake\", \"CMakeCache.txt\", \"Makefile\"]\n"
    "        }\n"
    "    ],\n"
    "    \"build_systems\":\n"
    "    [\n"
    "        {\n"
    "            \"name\": \"%s - all\",\n"
    "            \"shell_cmd\": \"cmake -G \\\"MinGW Makefiles\\\" && mingw32-make all\",\n"
    "            \"working_dir\": \"${project_path}\",\n"
    "            \"file_regex\": \"^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$\"\n"
    "        },\n"
    "        {\n"
    "            \"name\": \"%s - run\",\n"
    "            \"shell_cmd\": \"cmake -G \\\"MinGW Makefiles\\\" && mingw32-make all && start cmd /c \\\"bin\\\\%s.exe && pause\\\"\",\n"
    "            \"working_dir\": \"${project_path}\",\n"
    "            \"file_regex\": \"^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$\"\n"
    "        },\n"
    "        {\n"
    "            \"name\": \"%s - clean\",\n"
    "            \"shell_cmd\": \"cmake -G \\\"MinGW Makefiles\\\" && mingw32-make clean\",\n"
    "            \"working_dir\": \"${project_path}\",\n"
    "            \"file_regex\": \"^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$\"\n"
    "        }\n"
    "    ]\n"
    "}\n";

int main() {
    std::string dirName = currentDirName();
    const char *dirNameCStr = dirName.c_str();

    FILE *gitignoreFile = fopen(".gitignore", "w");
    fprintf(gitignoreFile, gitignore);

    FILE *cmakeListsFile = fopen("CMakeLists.txt", "w");
    fprintf(cmakeListsFile, cmakeLists, dirNameCStr, dirNameCStr);

    FILE *projectFile = fopen((dirName + ".sublime-project").c_str(), "w");
    fprintf(projectFile, project, dirNameCStr, dirNameCStr, dirNameCStr, dirNameCStr);

    CreateDirectory("src", nullptr);

    return 0;
}
