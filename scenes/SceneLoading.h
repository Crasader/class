#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__

#include "cocos2d.h"

#define DISPLAY_TIME_LOGO_SCENE 0.1
#define TRANSITION_TIME 0.5

#define POSITIONY_LOADING_LABEL 30.0f
#define FONT_SIZE_LOADING_LABEL 15.0f

class SceneLoading : public cocos2d::Scene
{
public:
	SceneLoading();
	virtual ~SceneLoading();
    virtual bool init();
	CREATE_FUNC(SceneLoading);

	void update(float dt);

private:
	void loading();
	void goToScenemanager();
};

#endif // __LOGO_SCENE_H__
