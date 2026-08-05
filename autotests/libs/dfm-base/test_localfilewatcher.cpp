// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file test_localfilewatcher.cpp
 * @brief Unit tests for LocalFileWatcher (localfilewatcher.cpp)
 */

#include <gtest/gtest.h>
#include <QTemporaryDir>
#include <QDir>
#include <QUrl>
#include <QIcon>
#include <mutex>

#include <dfm-base/base/urlroute.h>
#include <dfm-base/dfm_global_defines.h>
#include <dfm-base/file/local/localfilewatcher.h>

using namespace dfmbase;

class LocalFileWatcherTest : public testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        std::call_once(flag, [] {
            UrlRoute::regScheme(Global::Scheme::kFile, QDir::homePath(), QIcon(), false, "file");
        });
    }

    void SetUp() override
    {
        ASSERT_TRUE(tmpDir.isValid());
        rootPath = tmpDir.path();
    }

    QTemporaryDir tmpDir;
    QString rootPath;
    static std::once_flag flag;
};

std::once_flag LocalFileWatcherTest::flag;

TEST_F(LocalFileWatcherTest, ConstructNoCrash)
{
    QUrl url = QUrl::fromLocalFile(rootPath);
    EXPECT_NO_FATAL_FAILURE({ LocalFileWatcher watcher(url); });
}

TEST_F(LocalFileWatcherTest, UrlAccessor)
{
    QUrl url = QUrl::fromLocalFile(rootPath);
    LocalFileWatcher watcher(url);
    EXPECT_EQ(watcher.url().path(), url.path());
}

TEST_F(LocalFileWatcherTest, StartStopWatcher)
{
    QUrl url = QUrl::fromLocalFile(rootPath);
    LocalFileWatcher watcher(url);
    // start should succeed for existing dir
    EXPECT_TRUE(watcher.startWatcher());
    // already started
    EXPECT_TRUE(watcher.startWatcher());
    // stop
    EXPECT_TRUE(watcher.stopWatcher());
    // stop when not started
    EXPECT_TRUE(watcher.stopWatcher());
}

TEST_F(LocalFileWatcherTest, StartNonExistentDirFails)
{
    QUrl url = QUrl::fromLocalFile(rootPath + "/no/such/dir");
    LocalFileWatcher watcher(url);
    EXPECT_FALSE(watcher.startWatcher());
}

TEST_F(LocalFileWatcherTest, RestartWatcher)
{
    QUrl url = QUrl::fromLocalFile(rootPath);
    LocalFileWatcher watcher(url);
    EXPECT_TRUE(watcher.restartWatcher());
}

TEST_F(LocalFileWatcherTest, NotifyFileAddedEmitsSignal)
{
    QUrl url = QUrl::fromLocalFile(rootPath);
    LocalFileWatcher watcher(url);
    EXPECT_NO_FATAL_FAILURE({ watcher.notifyFileAdded(QUrl("file:///tmp/new")); });
}

TEST_F(LocalFileWatcherTest, NotifyFileChangedEmitsSignal)
{
    QUrl url = QUrl::fromLocalFile(rootPath);
    LocalFileWatcher watcher(url);
    EXPECT_NO_FATAL_FAILURE({ watcher.notifyFileChanged(QUrl("file:///tmp/changed")); });
}

TEST_F(LocalFileWatcherTest, NotifyFileDeletedEmitsSignal)
{
    QUrl url = QUrl::fromLocalFile(rootPath);
    LocalFileWatcher watcher(url);
    EXPECT_NO_FATAL_FAILURE({ watcher.notifyFileDeleted(QUrl("file:///tmp/gone")); });
}

TEST_F(LocalFileWatcherTest, CacheInfoConnectSize)
{
    QUrl url = QUrl::fromLocalFile(rootPath);
    LocalFileWatcher watcher(url);
    EXPECT_EQ(watcher.getCacheInfoConnectSize(), 0);
    watcher.addCacheInfoConnectSize();
    EXPECT_EQ(watcher.getCacheInfoConnectSize(), 1);
    watcher.reduceCacheInfoConnectSize();
    EXPECT_EQ(watcher.getCacheInfoConnectSize(), 0);
}
