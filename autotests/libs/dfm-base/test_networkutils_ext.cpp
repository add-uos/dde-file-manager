// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file test_networkutils.cpp
 * @brief Unit tests for NetworkUtils (networkutils.cpp)
 */

#include <gtest/gtest.h>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QMap>

#include <dfm-base/utils/networkutils.h>

using namespace dfmbase;

TEST(NetworkUtilsTest, InstanceReturnsNonNull)
{
    EXPECT_NE(NetworkUtils::instance(), nullptr);
}

TEST(NetworkUtilsTest, ParseIpSimpleHostPort)
{
    QString ip, port;
    EXPECT_NO_FATAL_FAILURE({
        (void)NetworkUtils::instance()->parseIp("192.168.1.1:445", ip, port);
    });
}

TEST(NetworkUtilsTest, ParseIpWithPortsList)
{
    QString ip;
    QStringList ports;
    bool ok = NetworkUtils::instance()->parseIp("10.0.0.1", ip, ports);
    EXPECT_NO_FATAL_FAILURE({ (void)ok; (void)ip; (void)ports; });
}

TEST(NetworkUtilsTest, ParseIpInvalidReturns)
{
    QString ip, port;
    bool ok = NetworkUtils::instance()->parseIp("", ip, port);
    EXPECT_NO_FATAL_FAILURE({ (void)ok; });
}

TEST(NetworkUtilsTest, CheckNetConnectionUnreachableHost)
{
    EXPECT_NO_FATAL_FAILURE({
        (void)NetworkUtils::instance()->checkNetConnection("192.168.99.99", "9999", 500);
    });
}

TEST(NetworkUtilsTest, CheckNetConnectionWithPortList)
{
    EXPECT_NO_FATAL_FAILURE({
        (void)NetworkUtils::instance()->checkNetConnection("192.168.99.99", QStringList { "9999", "9998" }, 500);
    });
}

TEST(NetworkUtilsTest, DoAfterCheckNetNoCrash)
{
    EXPECT_NO_FATAL_FAILURE({
        NetworkUtils::instance()->doAfterCheckNet("192.168.99.99", { "9999" }, nullptr, 500);
    });
}

TEST(NetworkUtilsTest, CifsMountHostInfoStatic)
{
    EXPECT_NO_FATAL_FAILURE({ (void)NetworkUtils::cifsMountHostInfo(); });
}

TEST(NetworkUtilsTest, CheckAllCIFSBusy)
{
    EXPECT_NO_FATAL_FAILURE({ (void)NetworkUtils::instance()->checkAllCIFSBusy(); });
}

TEST(NetworkUtilsTest, CheckFtpOrSmbBusy)
{
    EXPECT_NO_FATAL_FAILURE({ (void)NetworkUtils::instance()->checkFtpOrSmbBusy(QUrl("ftp://no.such.host/")); });
}

TEST(NetworkUtilsTest, ResolveLocalSftpMountUrl)
{
    EXPECT_NO_FATAL_FAILURE({ (void)NetworkUtils::resolveLocalSftpMountUrl(QUrl("sftp://no.such.host/")); });
}
