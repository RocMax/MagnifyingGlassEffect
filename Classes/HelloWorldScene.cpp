#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
    
    auto touchListener=EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan= CC_CALLBACK_2(HelloWorld::onTouchBegan,this);
    touchListener->onTouchMoved=CC_CALLBACK_2(HelloWorld::onTouchMoved , this);
    touchListener->onTouchEnded=CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("bg.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

Sprite* HelloWorld::getMaskedSprite(cocos2d::Rect rect){
    Sprite* textureSprite=Sprite::create("bg.png", rect);
    Sprite* maskSprite=Sprite::create("mask.png");
    
    //创建RenderTexture,尺寸符合mask
    RenderTexture * rt = RenderTexture::create( maskSprite->getContentSize().width,
                                               maskSprite->getContentSize().height );
    
    maskSprite->setPosition(maskSprite->getContentSize().width/2,
                            maskSprite->getContentSize().height/2);
    textureSprite->setPosition(textureSprite->getContentSize().width/2,
                               textureSprite->getContentSize().height/2);
    
    //对于mask,需要先渲染到RenderTexture上,源因子1,目标因子0
    maskSprite->setBlendFunc( BlendFunc{GL_ONE, GL_ZERO} );
    //对于原图,渲染的时候透明度采用mask的透明度,这样在mask透明的地方原图也透明,实现蒙板效果,目标因子0去掉mask
    textureSprite->setBlendFunc( BlendFunc{GL_DST_ALPHA, GL_ZERO} );
    
    //开始渲染,先是mask,然后原图
    rt->begin();
    maskSprite->visit();
    textureSprite->visit();
    rt->end();
    
    //提取sprite
    Sprite* final =Sprite::createWithTexture(rt->getSprite()->getTexture());
    //反转Y轴,纹理创建时Y轴是反的
    final->setFlippedY(true);
    return final;
}


bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    Rect rect=Rect(touch->getLocationInView().x-50, touch->getLocationInView().y-50, 100, 100);
    showdetail=getMaskedSprite(rect);
    showdetail->setScale(3.0f);
    showdetail->setPosition(touch->getLocation());
    addChild(showdetail);
    return  true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    Rect rect=Rect(touch->getLocation().x-50, touch->getLocationInView().y-50, 100, 100);
    showdetail->setTexture(getMaskedSprite(rect)->getTexture()) ;
    showdetail->setPosition(touch->getLocation());
}
void  HelloWorld::onTouchEnded(cocos2d::Touch *touch,cocos2d::Event *event){
    showdetail->removeFromParentAndCleanup(true);
}