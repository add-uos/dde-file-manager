// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file test_abstractbaseview.cpp
 * @brief Unit tests for AbstractBaseView default implementations.
 */

#include <gtest/gtest.h>
#include <QWidget>
#include <QAction>
#include <QUrl>
#include <QList>

#include <dfm-base/interfaces/abstractbaseview.h>

using namespace dfmbase;

class TestBaseView : public AbstractBaseView
{
public:
    QWidget *widget() const override { return m_widget; }
    QUrl rootUrl() const override { return m_rootUrl; }
    bool setRootUrl(const QUrl &url) override { m_rootUrl = url; return true; }
    QWidget *m_widget = nullptr;
    QUrl m_rootUrl;
};

TEST(AbstractBaseViewTest, ViewStateDefaultIsViewIdle)
{
    TestBaseView view;
    EXPECT_EQ(view.viewState(), AbstractBaseView::ViewState::kViewIdle);
}

TEST(AbstractBaseViewTest, ToolBarActionListEmpty)
{
    TestBaseView view;
    EXPECT_TRUE(view.toolBarActionList().isEmpty());
}

TEST(AbstractBaseViewTest, SelectedUrlListEmpty)
{
    TestBaseView view;
    EXPECT_TRUE(view.selectedUrlList().isEmpty());
}

TEST(AbstractBaseViewTest, ContentWidgetReturnsNull)
{
    TestBaseView view;
    EXPECT_EQ(view.contentWidget(), nullptr);
}

TEST(AbstractBaseViewTest, RefreshAndDeleteLaterNoCrash)
{
    TestBaseView view;
    EXPECT_NO_FATAL_FAILURE({ view.refresh(); });
    EXPECT_NO_FATAL_FAILURE({ view.deleteLater(); });
}

TEST(AbstractBaseViewTest, SetAndGetRootUrl)
{
    TestBaseView view;
    QUrl url("file:///tmp/test");
    EXPECT_TRUE(view.setRootUrl(url));
    EXPECT_EQ(view.rootUrl(), url);
}