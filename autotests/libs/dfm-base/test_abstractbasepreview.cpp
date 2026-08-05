// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file test_abstractbasepreview.cpp
 * @brief Unit tests for AbstractBasePreview default implementations.
 */

#include <gtest/gtest.h>
#include <QWidget>
#include <QUrl>
#include <QString>

#include <dfm-base/interfaces/abstractbasepreview.h>

using namespace dfmbase;

class TestBasePreview : public AbstractBasePreview
{
public:
    bool setFileUrl(const QUrl &url) override { m_url = url; return true; }
    QUrl fileUrl() const override { return m_url; }
    QWidget *contentWidget() const override { return nullptr; }
    QUrl m_url;
};

TEST(AbstractBasePreviewTest, InitializeNoCrash)
{
    TestBasePreview preview;
    EXPECT_NO_FATAL_FAILURE({ preview.initialize(nullptr, nullptr); });
}

TEST(AbstractBasePreviewTest, StatusBarWidgetReturnsNull)
{
    TestBasePreview preview;
    EXPECT_EQ(preview.statusBarWidget(), nullptr);
}

TEST(AbstractBasePreviewTest, StatusBarWidgetAlignment)
{
    TestBasePreview preview;
    EXPECT_EQ(preview.statusBarWidgetAlignment(), Qt::AlignCenter);
}

TEST(AbstractBasePreviewTest, TitleEmpty)
{
    TestBasePreview preview;
    EXPECT_TRUE(preview.title().isEmpty());
}

TEST(AbstractBasePreviewTest, ShowStatusBarSeparator)
{
    TestBasePreview preview;
    EXPECT_FALSE(preview.showStatusBarSeparator());
}

TEST(AbstractBasePreviewTest, PlayPauseStopNoCrash)
{
    TestBasePreview preview;
    EXPECT_NO_FATAL_FAILURE({ preview.play(); });
    EXPECT_NO_FATAL_FAILURE({ preview.pause(); });
    EXPECT_NO_FATAL_FAILURE({ preview.stop(); });
}

TEST(AbstractBasePreviewTest, HandleBeforDestroyNoCrash)
{
    TestBasePreview preview;
    EXPECT_NO_FATAL_FAILURE({ preview.handleBeforDestroy(); });
}

TEST(AbstractBasePreviewTest, SetFileUrlAndFileUrl)
{
    TestBasePreview preview;
    QUrl url("file:///tmp/test.txt");
    EXPECT_TRUE(preview.setFileUrl(url));
    EXPECT_EQ(preview.fileUrl(), url);
}
