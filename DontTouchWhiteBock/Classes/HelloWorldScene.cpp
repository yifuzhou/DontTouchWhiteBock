#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    
    auto layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	//�����������
	srand(time(NULL));

	visibleSize = Director::getInstance()->getVisibleSize();
	
	gameLayer = Node::create();
	addChild(gameLayer);
	
	timerLabel = Label::create();
	timerLabel->setColor(Color3B::BLUE);
	timerLabel->setSystemFontSize(48);
	timerLabel->setPosition(visibleSize.width / 2, visibleSize.height-100);
	timerLabel->setString("0.000");
	addChild(timerLabel);

	startGame();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* t, Event* e)
	{
		auto bs = Block::getBlocks();
		Block *b;
		for (auto it = bs->begin(); it!= bs->end(); it++)
		{
			b = *it;
			if (b->getLineIndex() == 1 &&
				b->getBoundingBox().containsPoint(t->getLocation()))
			{
				if (b->getColor() == Color3B::BLACK&&Count != BlockNum)
				{
					if (!timerRunning)
					{
						this->startTimer();
					}
					b->setColor(Color3B::GRAY);
					this->moveDown();
					log("<10");
				
				}
				else if (b->getColor() == Color3B::BLACK&&Count == BlockNum)
				{
					log("=10");
					b->setColor(Color3B::GRAY);
					this->moveDownTwice(); 
					
					this->stopTimer();
					
				}
				else MessageBox("GameOver", "Fail");
				break;
			}
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()->
		addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void HelloWorld::startGame()
{
	BlockNum = 10;
	showEnd = false;
	timerRunning = false;

	Count = 1;
	linesCount = 0;
	addStartLine();
	addNormalLine(1);
	addNormalLine(2);
	addNormalLine(3);

}

void HelloWorld::addStartLine()
{
	auto b = Block::createWithArgs(Color3B::YELLOW, Size(visibleSize.width, visibleSize.height / 4), "",
		20, Color4B::BLACK);
	gameLayer->addChild(b);
	b->setLineIndex(0);

}

void HelloWorld::addEndLine()
{
	auto b = Block::createWithArgs(Color3B::GREEN, visibleSize, "GameOver", 32,
		Color4B::BLACK);
	gameLayer->addChild(b);
	b->setLineIndex(4);
}

void HelloWorld::addNormalLine(int lineIndex)
{
	Block * b;
	int blackIndex = rand() % 4;

	for (int i = 0; i < 4; i++)
	{
		b = Block::createWithArgs(blackIndex == i ? Color3B::BLACK : Color3B::WHITE, Size
			(visibleSize.width / 4 - 1, visibleSize.height / 4 - 1), "", 20, Color4B::BLACK);
		gameLayer->addChild(b);

		b->setPosition(i*visibleSize.width / 4, lineIndex*visibleSize.height / 4);
		b->setLineIndex(lineIndex);
	}
	linesCount++;
}

void HelloWorld::moveDown()
{
	if (linesCount < BlockNum)
	{
		addNormalLine(4);
	}
	else if (!showEnd)
	{
		addEndLine();
		showEnd = true;
	}

	Count++;
	auto bs = Block::getBlocks();
	for (auto it = bs->begin(); it != bs->end(); it++)
	{
		(*it)->moveDown();
	}
}

void HelloWorld::moveDownTwice()
{
	
	auto bs = Block::getBlocks();
	for (auto it = bs->begin(); it != bs->end(); it++)
	{
		(*it)->moveDownTwice();
	}
}

void HelloWorld::update(float dt)
{
	long offset = clock() - startTime;
	timerLabel->setString(StringUtils::format("%g", ((double)offset) / 1000));

}

void HelloWorld::startTimer()
{
	if (!timerRunning)
	{
		scheduleUpdate();
		startTime = clock();
		timerRunning = true;
	}
}

void HelloWorld::stopTimer()
{
	if (timerRunning)
	{
		unscheduleUpdate();
		timerRunning = false;
	}
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
