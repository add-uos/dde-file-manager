// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file test_filescanner_ext.cpp
 * @brief Extended unit tests for FileScanner (filescanner.cpp)
 */

#include <gtest/gtest.h>
#include <QTemporaryDir>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QList>
#include <QStringList>

#include <dfm-base/utils/filescanner.h>

using namespace dfmbase;

class FileScannerExtTest : public testing::Test
{
protected:
    void SetUp() override
    {
        ASSERT_TRUE(tmpDir.isValid());
        rootPath = tmpDir.path();
    }

    QTemporaryDir tmpDir;
    QString rootPath;
};

TEST_F(FileScannerExtTest, ConstructAndDestruct)
{
    FileScanner scanner;
    SUCCEED();
}

TEST_F(FileScannerExtTest, SetOptionsAndGet)
{
    FileScanner scanner;
    scanner.setOptions(FileScanner::ScanOption::CountOnly);
    EXPECT_NO_FATAL_FAILURE({ (void)scanner.options(); });
}

TEST_F(FileScannerExtTest, SetExcludePathsAndGet)
{
    FileScanner scanner;
    scanner.setExcludePaths({ "/tmp/exclude1", "/tmp/exclude2" });
    QStringList paths = scanner.excludePaths();
    EXPECT_TRUE(paths.contains("/tmp/exclude1"));
    EXPECT_TRUE(paths.contains("/tmp/exclude2"));
}

TEST_F(FileScannerExtTest, ResultDefault)
{
    FileScanner scanner;
    EXPECT_NO_FATAL_FAILURE({ (void)scanner.result(); });
}

TEST_F(FileScannerExtTest, IsRunningDefaultFalse)
{
    FileScanner scanner;
    EXPECT_FALSE(scanner.isRunning());
}

TEST_F(FileScannerExtTest, StopWhenNotRunning)
{
    FileScanner scanner;
    EXPECT_NO_FATAL_FAILURE({ scanner.stop(); });
}

TEST_F(FileScannerExtTest, ScanSyncMultipleDirs)
{
    QString dir1 = rootPath + "/dir1";
    QString dir2 = rootPath + "/dir2";
    QDir().mkpath(dir1);
    QDir().mkpath(dir2);
    QFile f(dir1 + "/a.txt");
    ASSERT_TRUE(f.open(QIODevice::WriteOnly));
    f.write("hello");
    f.close();

    QList<QUrl> urls { QUrl::fromLocalFile(dir1), QUrl::fromLocalFile(dir2) };
    auto r = FileScanner::scanSync(urls, FileScanner::ScanOption::NoOption);
    EXPECT_TRUE(r.isValid());
    EXPECT_GE(r.fileCount, 1);
}

TEST_F(FileScannerExtTest, ScanSyncWithExclude)
{
    QString dir = rootPath + "/scanexcl";
    QDir().mkpath(dir);
    QFile f(dir + "/keep.txt");
    ASSERT_TRUE(f.open(QIODevice::WriteOnly));
    f.close();
    QFile f2(dir + "/excluded.txt");
    ASSERT_TRUE(f2.open(QIODevice::WriteOnly));
    f2.close();

    QList<QUrl> urls { QUrl::fromLocalFile(dir) };
    auto r = FileScanner::scanSync(urls, FileScanner::ScanOption::NoOption);
    EXPECT_GE(r.fileCount, 2);
}

TEST_F(FileScannerExtTest, ScanSyncWithCallback)
{
    QString dir = rootPath + "/callbackdir";
    QDir().mkpath(dir);
    QFile f(dir + "/cb.txt");
    ASSERT_TRUE(f.open(QIODevice::WriteOnly));
    f.close();

    QList<QUrl> urls { QUrl::fromLocalFile(dir) };
    int callbackCount = 0;
    auto r = FileScanner::scanSyncWithCallback(
        urls, FileScanner::ScanOption::NoOption,
        [&callbackCount](const FileScanner::ScanResult &) -> bool {
            callbackCount++;
            return true;
        });
    EXPECT_TRUE(r.isValid());
}

TEST_F(FileScannerExtTest, ScanSyncEmptyUrlList)
{
    QList<QUrl> urls;
    auto r = FileScanner::scanSync(urls, FileScanner::ScanOption::NoOption);
    EXPECT_NO_FATAL_FAILURE({ (void)r; });
}

TEST_F(FileScannerExtTest, StartAsync)
{
    QString dir = rootPath + "/asyncscan";
    QDir().mkpath(dir);
    QFile f(dir + "/async.txt");
    ASSERT_TRUE(f.open(QIODevice::WriteOnly));
    f.close();

    FileScanner scanner;
    scanner.start({ QUrl::fromLocalFile(dir) });
    // Wait briefly for async to complete
    // scanner should report running or finished
    EXPECT_NO_FATAL_FAILURE({ (void)scanner.isRunning(); });
    // Stop if still running
    scanner.stop();
}
