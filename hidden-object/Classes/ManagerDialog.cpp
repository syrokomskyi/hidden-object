#include "stdafx.h"
#include "ManagerDialog.h"
#include "ManagerScreen.h"
#include "AppDelegate.h"
#include "AboutRoomDialog.h"
#include "NeedEnergyDialog.h"
#include "ResultRoomDialog.h"


ManagerDialog*  ManagerDialog::mInstance = nullptr;




ManagerDialog::ManagerDialog( const Resource&  selectedResource ):
    selectedResource( selectedResource ),
    mNeedEnergyDialog()
{
    DASSERT( !selectedResource.directory.empty() );
}




ManagerDialog::~ManagerDialog() {
}




void
ManagerDialog::showAboutRoom() {

    if ( mAboutRoomDialog ) { mAboutRoomDialog.reset(); }
    mAboutRoomDialog =
        std::shared_ptr< AboutRoomDialog >( new AboutRoomDialog() );
    mAboutRoomDialog->dialog();
}




void
ManagerDialog::showResultRoom() {

    if ( mResultRoomDialog ) { mResultRoomDialog.reset(); }
    mResultRoomDialog =
        std::shared_ptr< ResultRoomDialog >( new ResultRoomDialog() );
    mResultRoomDialog->dialog();
}




void
ManagerDialog::showNeedEnergy() {

    if ( mNeedEnergyDialog ) { mNeedEnergyDialog.reset(); }
    mNeedEnergyDialog =
        std::shared_ptr< NeedEnergyDialog >( new NeedEnergyDialog() );
    mNeedEnergyDialog->dialog();
}
