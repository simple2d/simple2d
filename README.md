# Simple 2D

A simple 2D graphics engine. [Check out the website »](http://www.simple2d.com)

Build and install with:

```
make && make install
```

## Running Tests

### Getting Test Media

To keep the size of this repository small, media needed for tests are checked into the [`test_media`](https://github.com/simple2d/test_media) repo and referenced as a [Git submodule](http://git-scm.com/book/en/v2/Git-Tools-Submodules). After cloning this repo, init the submodule and get its contents using:

```
git submodule init
git submodule update --remote
```

Alternatively, you can clone the repo and update the submodule in one step using:

```
git clone --recursive https://github.com/simple2d/simple2d.git
```

Simply run `git submodule update --remote` anytime to get the latest changes from `test_media` (i.e. when there's a new commit available).

### Building the Tests

Simply run `make tests`. Built tests are placed in the `tests/` directory with the same name as their C source file.

### Available Tests

- [`triangle.c`](tests/triangle.c) – The "hello triangle" example from the [simple2d.com](http://www.simple2d.com) website.
- [`testcard.c`](tests/testcard.c) – A graphical card similar to [testcards in TV](http://en.wikipedia.org/wiki/Testcard), testing a wide range of visual capabilities.
- [`audio.c`](tests/audio.c) – Tests audio functions with different file formats interpreted as samples and music.

Run a test using `make tests && cd tests/ && ./<name_of_test>`, for example:

```
make tests && cd tests/ && ./triangle
```

Or, conveniently run tests using `bash test.sh <name_of_test>`, for example:

```
# Run triangle.c
bash test.sh triangle

# Run audio.c
bash test.sh audio
```
