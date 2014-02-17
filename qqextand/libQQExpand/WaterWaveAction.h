#pragma once
#include "cocos2d.h"

USING_NS_CC;

/**
* 波纹特效
*
* grid 的大小不要超过short的范围
*
 */
class CC_DLL WaterWaveAction : public Grid3DAction
{
public:

	WaterWaveAction(void);
	~WaterWaveAction(void);

    /** creates an action with duration, grid size */
    static WaterWaveAction* create(float duration, const Size& gridSize);

    /** initializes an action with duration, grid size */
    bool initWithDuration(float duration, const Size& gridSize);

	// Overrides
	virtual WaterWaveAction* clone() const override;
    virtual void update(float time) override;

	//计算波能数据缓冲区 
	void RippleSpread();

	void dropStone(int x, int y, float value);

	virtual GridBase* getGrid();

	void setColor(const Point& position, const Color4F& color);

protected:
	
	GLvoid *buff1;
	GLvoid *buff2;

	int _bo_length;
	int _grid_count;
};

/**
* Grid3D is a 3D grid implementation. Each vertex has 3 dimensions: x,y,z
*
* grid 的大小不要超过short的范围
*
 */
class CC_DLL GridColor3D : public Grid3D
#ifdef EMSCRIPTEN
, public GLBufferedNode
#endif // EMSCRIPTEN
{
public:
    /** create one Grid */
    static GridColor3D* create(const Size& gridSize, Texture2D *texture, bool bFlipped);
    /** create one Grid */
    static GridColor3D* create(const Size& gridSize);
    /**
     * @js ctor
     */
    GridColor3D();
    /**
     * @js NA
     * @lua NA
     */
    ~GridColor3D(void);

    // Overrides
    virtual void blit() override;
    virtual void reuse() override;
    virtual void calculateVertexPoints() override;

	Color4F getGridColor(const Point& pos) const;
	GLvoid* getGridColor() const;
	void setGridColor(const Point& pos, const Color4F& color);
	GLvoid* getOriginalVertex();

protected:
    GLvoid *_colors;
};