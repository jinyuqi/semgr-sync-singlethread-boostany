#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "semanager.h"

class TestMgr : public SEManager
{
public:
    TestMgr(int id) : SEManager(id) {}
    virtual void subInitializeCallbak() {}
    virtual void IntsubInitializeCallbak(const int& value) {}
    virtual void secsubInitializeCallbak() {}
    virtual void thirdsubInitializeCallbak() {}
};

class MockTestMgr : public TestMgr
{
public:
    MockTestMgr(int id) : TestMgr(id) {}
    MOCK_METHOD0(subInitializeCallbak, void());
    MOCK_METHOD0(secsubInitializeCallbak, void());
    MOCK_METHOD0(thirdsubInitializeCallbak, void());
    MOCK_METHOD1(IntsubInitializeCallbak, void(const int &));
};

TEST(SEMGRTEST, subscribeAndPostWillDoSingleCallback)
{
    SEManager::clearAllSubscribe();
    MockTestMgr mgr(0), mgr2(1);
    EXPECT_CALL(mgr, subInitializeCallbak()).Times(1);
    mgr.subscribe(MsgData::Initialize, std::bind(&MockTestMgr::subInitializeCallbak, &mgr));
    mgr2.postMsg(MsgData::Initialize);
}

TEST(SEMGRTEST, subscribeTwiceAndPostWillDoOnceCallback)
{
    SEManager::clearAllSubscribe();
    MockTestMgr mgr(0), mgr2(1);
    EXPECT_CALL(mgr, subInitializeCallbak()).Times(1);
    mgr.subscribe(MsgData::Initialize, std::bind(&MockTestMgr::subInitializeCallbak, &mgr));
    mgr.subscribe(MsgData::Initialize, std::bind(&MockTestMgr::subInitializeCallbak, &mgr));
    mgr2.postMsg(MsgData::Initialize);
}

TEST(SEMGRTEST, noSubscribeAndPostWillNoCallback)
{
    SEManager::clearAllSubscribe();
    MockTestMgr mgr(0), mgr2(1);
    EXPECT_CALL(mgr, subInitializeCallbak()).Times(0);
    mgr2.postMsg(MsgData::Initialize);
}

TEST(SEMGRTEST, eachSubscribeAndWillCallEachCallBack)
{
    SEManager::clearAllSubscribe();
    MockTestMgr mgr(0), mgr2(1);
    EXPECT_CALL(mgr, subInitializeCallbak()).Times(1);
    EXPECT_CALL(mgr2, subInitializeCallbak()).Times(1);
    mgr.subscribe(MsgData::Initialize, std::bind(&MockTestMgr::subInitializeCallbak, &mgr));
    mgr2.subscribe(MsgData::Initialize, std::bind(&MockTestMgr::subInitializeCallbak, &mgr2));
    mgr.postMsg(MsgData::Initialize);
}

TEST(SEMGRTEST, SubscribeAndNoSubscribeWithTwoMgrs)
{
    SEManager::clearAllSubscribe();
    MockTestMgr mgr(0), mgr2(1);
    EXPECT_CALL(mgr, subInitializeCallbak()).Times(1);
    EXPECT_CALL(mgr2, subInitializeCallbak()).Times(0);
    mgr.subscribe(MsgData::Initialize, std::bind(&MockTestMgr::subInitializeCallbak, &mgr));
    mgr.postMsg(MsgData::Initialize);
}

TEST(SEMGRTEST, SubscribeAndNoSubscribeWithOneMgrs)
{
    SEManager::clearAllSubscribe();
    MockTestMgr mgr(0), mgr2(1);
    EXPECT_CALL(mgr, subInitializeCallbak()).Times(0);
    mgr.subscribe(MsgData::Initialize, std::bind(&MockTestMgr::subInitializeCallbak, &mgr));
    mgr.unSubscribe(MsgData::Initialize);
    mgr.postMsg(MsgData::Initialize);
}

TEST(SEMGRTEST, NoSubscribeWithOneMgrs)
{
    SEManager::clearAllSubscribe();
    MockTestMgr mgr(0), mgr2(1);
    EXPECT_CALL(mgr, subInitializeCallbak()).Times(0);
    mgr.unSubscribe(MsgData::Initialize);
    mgr.postMsg(MsgData::Initialize);
}

TEST(SEMGRTEST, ThreeManagersWithSubscribe)
{
    SEManager::clearAllSubscribe();
    MockTestMgr mgr(0), mgr2(1), mgr3(2);
    mgr.subscribe(MsgData::Initialize, std::bind(&MockTestMgr::subInitializeCallbak, &mgr));
    EXPECT_CALL(mgr, subInitializeCallbak()).Times(1).WillOnce(testing::Invoke([&](){mgr3.postMsg(MsgData::TEST_DOWORK);}));
    EXPECT_CALL(mgr2, secsubInitializeCallbak()).Times(1);
    mgr2.subscribe(MsgData::TEST_DOWORK, std::bind(&MockTestMgr::secsubInitializeCallbak, &mgr2));
    mgr3.postMsg(MsgData::Initialize);
}

TEST(SEMGRTEST, SupportPassArgument)
{
    SEManager::clearAllSubscribe();
    MockTestMgr mgr(0), mgr2(1);
    mgr.subscribe<int>(MsgData::Initialize, std::bind(&MockTestMgr::IntsubInitializeCallbak, &mgr, std::placeholders::_1));

    EXPECT_CALL(mgr, IntsubInitializeCallbak(5)).Times(1);
    mgr2.postMsg(MsgData::Initialize, 5);
}

//TEST(SEMGRTEST, SupportPassArgumentForTwoTimes)
//{
//    SEManager::clearAllSubscribe();
//    MockTestMgr mgr(0), mgr2(1);
//    mgr.subscribe<int>(MsgData::Initialize, std::bind(&MockTestMgr::IntsubInitializeCallbak, &mgr, std::placeholders::_1));
//
//    EXPECT_CALL(mgr, IntsubInitializeCallbak(5)).Times(1);
//    EXPECT_CALL(mgr, IntsubInitializeCallbak(3)).Times(1);
//    mgr2.postMsg(MsgData::Initialize, 5);
//    mgr2.postMsg(MsgData::Initialize, 3);
//}
