#ifndef MENU_APP_H
#define MENU_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <app.h>
#include <apps_manager.h>

AppSpecification_t* MenuAppSpecification(const _u16 appId,
                                         AppsManager_t* appsManager);

#ifdef __cplusplus
}
#endif

#endif  // MENU_APP_H