#include <windows.h>
#include <direct.h>
#include <iostream>
#include <memory>

std::string currentDirName() {
    char buffer[FILENAME_MAX];

    if (!_getcwd(buffer, sizeof(buffer)))
        return "";

    std::string str(buffer);
    std::string::size_type pos = str.find_last_of("\\") + 1;

    return str.substr(pos);
}

std::string exec(const char *cmd) {
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);

    if (!pipe)
        throw std::runtime_error("popen() failed!");

    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }

    return result;
}

static const char *gitignore =
    "bin/\n"
    "CMakeFiles/\n"
    "cmake_install.cmake\n"
    "CmakeCache.txt\n"
    "*.sublime-workspace\n"
    "Makefile\n";

static const char *cmakeLists =
    "cmake_minimum_required(VERSION %s)\n"
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

    std::string version = exec("cmake --version");
    version = version.substr(version.find_first_of(' ') + 1);
    version = version.substr(version.find_first_of(' ') + 1);
    version = version.substr(0, version.find_first_of('\n'));

    FILE *gitignoreFile = fopen(".gitignore", "w");
    fprintf(gitignoreFile, gitignore);

    FILE *cmakeListsFile = fopen("CMakeLists.txt", "w");
    fprintf(cmakeListsFile, cmakeLists, version.c_str(), dirNameCStr, dirNameCStr);

    FILE *projectFile = fopen((dirName + ".sublime-project").c_str(), "w");
    fprintf(projectFile, project, dirNameCStr, dirNameCStr, dirNameCStr, dirNameCStr);

    CreateDirectory("src", nullptr);

    return 0;
}
