#include "plotly/plotly.hpp"
#include <atomic>
#include <condition_variable>
#include <gtest/gtest.h>
#include <mutex>

namespace plotly {

class FigureTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

// Test basic Figure construction
TEST_F(FigureTest, Construction) {
  EXPECT_NO_THROW({ Figure fig; });
}

// Test browser operations
TEST_F(FigureTest, BrowserOperations) {
  Figure fig;

  // Test initial state
  EXPECT_FALSE(fig.isOpen());

  // Test opening browser in headless mode
  bool opened = fig.openBrowser(true);
  if (opened) {
    EXPECT_TRUE(fig.isOpen());
  }
}

// Test plot creation and manipulation
TEST_F(FigureTest, PlotCreationAndManipulation) {
  Figure fig;

  bool browserOpened = fig.openBrowser(true);
  if (!browserOpened) {
    GTEST_SKIP() << "Browser could not be opened, skipping plot tests";
  }

  // Create simple plot data
  Object trace = {{"x", Object::array({1, 2, 3, 4})},
                  {"y", Object::array({10, 11, 12, 13})},
                  {"type", "scatter"},
                  {"mode", "lines+markers"}};

  Object data = Object::array({trace});
  Object layout = Object{{"title", "Test Plot"}};

  // Test newPlot
  EXPECT_TRUE(fig.newPlot(data, layout));

  // Test update
  Object traceUpdate = {{"marker.color", "red"}};
  EXPECT_TRUE(fig.update(traceUpdate));

  // Test relayout
  Object layoutUpdate = {{"title", "Updated Test Plot"}};
  EXPECT_TRUE(fig.relayout(layoutUpdate));

  // Test redraw
  EXPECT_TRUE(fig.redraw());
}

// Test trace operations
TEST_F(FigureTest, TraceOperations) {
  Figure fig;

  bool browserOpened = fig.openBrowser(true);
  if (!browserOpened) {
    GTEST_SKIP() << "Browser could not be opened, skipping trace tests";
  }

  // Create initial plot
  Object trace1 = {{"x", Object::array({1, 2, 3})},
                   {"y", Object::array({1, 4, 9})},
                   {"type", "scatter"},
                   {"name", "trace1"}};

  Object data = Object::array({trace1});
  EXPECT_TRUE(fig.newPlot(data));

  // Test addTraces
  Object trace2 = {{"x", Object::array({1, 2, 3})},
                   {"y", Object::array({2, 5, 10})},
                   {"type", "scatter"},
                   {"name", "trace2"}};

  EXPECT_TRUE(fig.addTraces(Object::array({trace2})));

  // Test restyle
  Object styleUpdate = {{"marker.color", "blue"}};
  EXPECT_TRUE(fig.restyle(styleUpdate, Object::array({0})));

  // Test extendTraces
  Object extendData = {{"x", Object::array({4})}, {"y", Object::array({16})}};
  EXPECT_TRUE(fig.extendTraces(extendData, Object::array({0})));

  // Test moveTraces
  EXPECT_TRUE(fig.moveTraces(Object::array({0}), Object::array({1})));

  // Test deleteTraces
  EXPECT_TRUE(fig.deleteTraces(Object::array({1})));
}

// Test animation operations
TEST_F(FigureTest, AnimationOperations) {
  Figure fig;

  bool browserOpened = fig.openBrowser(true);
  if (!browserOpened) {
    GTEST_SKIP() << "Browser could not be opened, skipping animation tests";
  }

  // Simple test data
  Object initialTrace = {{"x", Object::array({1, 2, 3})},
                         {"y", Object::array({1, 2, 3})},
                         {"type", "scatter"}};

  Object data = Object::array({initialTrace});
  EXPECT_TRUE(fig.newPlot(data));

  // Test addFrames with simple frame data
  std::vector<Object> frames;
  for (int i = 0; i < 3; i++) {
    Object frameObj = {{"name", std::to_string(i)},
                       {"data", std::vector<Object>{Object{
                                    {"x", Object::array({1, 2, 3})},
                                    {"y", Object::array({i + 1, i + 2, i + 3})},
                                    {"type", "scatter"}}}}};
    frames.push_back(frameObj);
  }

  EXPECT_TRUE(fig.addFrames(frames));

  // Test animate - basic functionality
  EXPECT_TRUE(fig.animate(Object(), Object{}));
  EXPECT_TRUE(fig.animate(Object::array({"0", "1"}), Object{}));
  EXPECT_TRUE(fig.animate(Object::array({}), Object{})); // pause

  // Test deleteFrames
  EXPECT_TRUE(fig.deleteFrames(Object::array({"0", "1", "2"})));
}

// Test react operation
TEST_F(FigureTest, ReactOperation) {
  Figure fig;

  bool browserOpened = fig.openBrowser(true);
  if (!browserOpened) {
    GTEST_SKIP() << "Browser could not be opened, skipping react tests";
  }

  // Test react with new data
  Object trace = {{"x", Object::array({1, 2, 3})},
                  {"y", Object::array({1, 4, 9})},
                  {"type", "bar"}};

  Object data = Object::array({trace});
  Object layout = {{"title", "React Test"}};
  Object config = {{"displayModeBar", false}};

  EXPECT_TRUE(fig.react(data, layout, config));
}

// Test purge operation
TEST_F(FigureTest, PurgeOperation) {
  Figure fig;

  bool browserOpened = fig.openBrowser(true);
  if (!browserOpened) {
    GTEST_SKIP() << "Browser could not be opened, skipping purge tests";
  }

  // Create a plot first
  Object trace = {{"x", Object::array({1, 2, 3})},
                  {"y", Object::array({1, 4, 9})},
                  {"type", "scatter"}};

  Object data = Object::array({trace});
  EXPECT_TRUE(fig.newPlot(data));

  // Test purge
  EXPECT_TRUE(fig.purge());
}

// Test event handling
TEST_F(FigureTest, EventHandling) {
  Figure fig;

  bool browserOpened = fig.openBrowser(true);
  if (!browserOpened) {
    GTEST_SKIP() << "Browser could not be opened, skipping event tests";
  }

  // Create a plot to generate events
  Object trace = {{"x", Object::array({1, 2, 3})},
                  {"y", Object::array({1, 4, 9})},
                  {"type", "scatter"}};

  Object data = Object::array({trace});
  EXPECT_TRUE(fig.newPlot(data));

  // Test event listener registration
  std::atomic<bool> eventReceived{false};
  std::mutex eventMutex;
  std::condition_variable eventCv;

  bool registered =
      fig.on("plotly_click", [&](const plotly::Object & /* eventData */) {
        std::lock_guard<std::mutex> lock(eventMutex);
        eventReceived = true;
        eventCv.notify_all();
      });

  EXPECT_TRUE(registered);

  // Test removing event listeners
  EXPECT_TRUE(fig.removeAllListeners("plotly_click"));
}

// Test download directory setting
TEST_F(FigureTest, DownloadDirectoryOperation) {
  Figure fig;

  bool browserOpened = fig.openBrowser(true);
  if (!browserOpened) {
    GTEST_SKIP()
        << "Browser could not be opened, skipping download directory tests";
  }

  // Test setting download directory
  std::filesystem::path downloadPath = "/tmp";
  // Note: This might fail if Chrome DevTools is not available, which is okay
  // We're just testing that the method doesn't crash
  EXPECT_NO_THROW({ fig.setDownloadDirectory(downloadPath); });
}

// Test image download
TEST_F(FigureTest, ImageDownload) {
  Figure fig;

  bool browserOpened = fig.openBrowser(true);
  if (!browserOpened) {
    GTEST_SKIP() << "Browser could not be opened, skipping download tests";
  }

  // Create a plot to download
  Object trace = {{"x", Object::array({1, 2, 3, 4})},
                  {"y", Object::array({10, 11, 12, 13})},
                  {"type", "scatter"},
                  {"mode", "lines+markers"}};

  Object data = Object::array({trace});
  Object layout = Object{{"title", "Download Test Plot"}};

  EXPECT_TRUE(fig.newPlot(data, layout));

  // Test different download options
  Object pngOpts = Object{{"format", "png"},
                          {"width", 800},
                          {"height", 600},
                          {"filename", "test_plot_png"}};

  EXPECT_NO_THROW({ fig.downloadImage(pngOpts); });

  Object svgOpts = Object{{"format", "svg"},
                          {"width", 800},
                          {"height", 600},
                          {"filename", "test_plot_svg"}};

  EXPECT_NO_THROW({ fig.downloadImage(svgOpts); });

  Object jpegOpts = Object{{"format", "jpeg"},
                           {"width", 800},
                           {"height", 600},
                           {"filename", "test_plot_jpeg"}};

  EXPECT_NO_THROW({ fig.downloadImage(jpegOpts); });

  Object pdfOpts = Object{{"format", "pdf"},
                          {"width", 800},
                          {"height", 600},
                          {"filename", "test_plot_pdf"}};

  EXPECT_NO_THROW({ fig.downloadImage(pdfOpts); });
}

// Test basic plotting workflow with headless browser
TEST_F(FigureTest, BasicPlottingWorkflow) {
  Figure fig;

  // Open browser in headless mode
  bool browserOpened = fig.openBrowser(true);

  if (browserOpened) {
    // Create simple plot data
    Object trace = {{"x", Object::array({1, 2, 3, 4})},
                    {"y", Object::array({10, 11, 12, 13})},
                    {"type", "scatter"},
                    {"mode", "lines+markers"}};

    Object data = Object::array({trace});

    Object layout = Object{{"title", "Test Plot"}};

    // Test newPlot - should not throw and should succeed
    EXPECT_NO_THROW({
      bool result = fig.newPlot(data, layout);
      EXPECT_TRUE(result);
    });

    // Test downloadImage - should not throw
    Object opts = Object{{"format", "png"},
                         {"width", 800},
                         {"height", 600},
                         {"filename", "test_plot"}};

    EXPECT_NO_THROW({ fig.downloadImage(opts); });
  }
}

} // namespace plotly
