# dss_homework
Implement the DSS take-home test

# Build
## Dependencies
Only known external dependencies are:
1. Visual Studio 2017 or Visual Studio 2019
    * MSVC Packages: vs150 & vs 160
* CMake 3.12.0 or greater
    * NOTE: I tested with 3.16.1
    * NOTE: I don't know the exact minimum CMake version based on the features I used

## Build
Below instructions are for building via CLI. CMake GUI will work as well, but can use the info below to guide that process
1. Clone the repository to your local disk, this path is `${REPO_ROOT}`
2. Open a terminal (`cmd.exe`, `powershell`, `git bash`) and change into `${REPO_ROOT}/build`
    - NOTE: If for some reason the build folder is not present, just create it
3. Run CMake to generate project files: `cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=<BUILD_TYPE> ..`
    - NOTE: Supported `BUILD_TYPE` are: `DEBUG` or `RELEASE`. If you want to build one, then the other, make sure to re-generate the build files
    - NOTE: If doing this multiple times, make sure build folder is cleared out before running this step
4. Build the project: `cmake --build . --config <BUILD_TYPE>`
    - Match the `BUILD_TYPE` from above. Pick either `Debug` or `Release`
5. If successful, the build artifacts (including necessary resources, and DLLs) are present at: `${REPO_ROOT}/build/bin/<BUILD_TYPE>`

If there are any issues or confusion, take a look at `build.bat` in the repo root. Travis CI runs this build script to generate artifacts.

### Tested Configurations:
Windows 10 RS5 (17763.864) with Visual Studio 2017, CMake 3.16.1, Release Build

CI Environment: Windows Server 1803 with Visual Studio 2017, CMake 3.12.3, Release Build

# Run
Two important things to be aware of:

1. You can run the application from `explorer` by double-clicking `DSSHomework.exe` in the build output folder. OR: You can run the application from a terminal by _changing_ into the build folder, then running like: `./DSSHomework.exe`
    - NOTE: Both methods above will ensure the DLLs next to the .exe are picked up when loading the application
2. If you are behind a proxy, you must set your proxy details when launching the application (either on the CLI, or through Windows). CLI Example: `./DSSHomework.exe example-proxy.com 80`
    - NOTE: Notice the proxy host is separate from the proxy port. Do not include the protocol (`http` or `https`)

# Releases
Releases are automatically created by pushing a tagged commit to the repo. Travis CI will pick these commits up, create a build, package and deploy it to the repos Releases page: https://github.com/keaton-freude/dss_homework/releases

The details in the above `Run` section are still relevant for these releases

# Details
Once the application is running, the window should have a background drawn across the entire content area. At this point, we've kicked off a background thread which will query the MLB Stats API for game data for the date: `2018-06-10`. Once the data has been retrieved and parsed into a JSON object, we kick off a thread which downloads one image per gate.

As the image data comes in, we pass it and other info (headline, blurb) to all registered observers of the MLB Stats Fetcher.

There is an observer of this data registered in the Application startup which will take this data and insert it into a work-queue which is then processed on the main thread.

### Aside: Why process on the main thread?
    I realized a little too late into the process that only the thread associated
    with the registered OpenGL context could create OpenGL resources for use with
    the main context. If I were to refactor this, I would probably do:

    Create a thread which creates its own OpenGL context, shared with the main-context.
    This thread would manage the creation of the `ContentTile`s, then only do a
    _move_ of the `ContentTile` into the `ContentTileList`. This means we'd only
    lock up the Render thread on a single move of an already-constructed `ContentTile`
    into the `ContentTileList`

The main thread will pull items off the work-queue and insert them into the `ContentTileList`. Each time items are added to the `ContentTileList`, or a new `ContentTile` is selected, we will re-calculate the positioning of each item in the `ContentTileList`.

NOTE: The layout management of the `ContentTile`s was a huge pain. If I were to refactor this, I'd create a simple layout-management system (something like Qt's `QHBoxLayout` or similar)

Each frame we process input, poke `ImGUI` to keep it moving, render our components and check if any items are available to be processed in the work queue

# Extras Branch
This branch contains refactorings, extra-credit type stuff and other things I wanted to work on. All of this work came _after_ the submission and is just for fun

## Layout System
Adding a layout system, so we can encapsulate and isolate the layout logic into its own set of classes. This lets us pull that layout logic out of the `ContentTileList`, which makes it more reusable and cleaner and ultimately more powerful.

