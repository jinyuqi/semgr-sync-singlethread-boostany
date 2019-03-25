#include "semanager.h"
#include <iostream>
using namespace std;

struct SubscribeMgr : public SEManager
{
    void onSubTest(double value)
    {
        cout << "double is " << value;
    }
};

struct PostMgr : public SEManager
{
};

int main()
{
    SubscribeMgr subMgr;
    subMgr.subscribe<double>(Msg::Initialize, std::bind(&SubscribeMgr::onSubTest, &subMgr, std::placeholders::_1));

    PostMgr postMgr;
    postMgr.postMsg(Msg::Initialize);

    return 0;
}
