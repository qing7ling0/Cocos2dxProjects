#include "WaterWaveAction.h"

WaterWaveAction::WaterWaveAction(void):buff1(nullptr), buff2(nullptr)
{
}

WaterWaveAction::~WaterWaveAction(void)
{
	CC_SAFE_FREE(buff1);
	CC_SAFE_FREE(buff2);
}


WaterWaveAction* WaterWaveAction::create(float duration, const Size& gridSize)
{
    WaterWaveAction *pAction = new WaterWaveAction();

    if (pAction)
    {
        if (pAction->initWithDuration(duration, gridSize))
        {
            pAction->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pAction);
        }
    }

    return pAction;    
}

bool WaterWaveAction::initWithDuration(float duration, const Size& gridSize)
{
    if (Grid3DAction::initWithDuration(duration, gridSize))
    {
		int size = (_gridSize.width+1) * (_gridSize.height+1);

		buff1 = malloc(size * sizeof(float));
		buff2 = malloc(size * sizeof(float));
		float *vertArray1 = (float*)buff1;
		float *vertArray2 = (float*)buff2;
		int i;
		
		_grid_count = size;
		_bo_length = _gridSize.height+1;
		for(i=0; i<_grid_count; i++)
		{
			vertArray1[i] = 0;
			vertArray2[i] = 0;
		}

        return true;
    }

    return false;
}

WaterWaveAction* WaterWaveAction::clone() const
{
	// no copy constructor
	auto a = new WaterWaveAction();
    a->initWithDuration(_duration, _gridSize);
	a->autorelease();
	return a;
}

void WaterWaveAction::update(float time)
{
	RippleSpread();
	float *vertArray2 = (float*)buff2;
    int i, j;

	float *oriVerArray = (float*)(((GridColor3D*)_target->getGrid())->getOriginalVertex());
	float *colorArray = (float*)(((GridColor3D*)_target->getGrid())->getGridColor());

	int verIndex = 0;
	int colIndex = 0;
	
	for (i = 0; i < _gridSize.width+1; ++i)
	{
		for (j = 0; j < _gridSize.height+1; ++j)
		{
			int index = i*(_gridSize.height+1)+j;
			float value = vertArray2[index];
			verIndex = index*3;
		
			oriVerArray[verIndex+2] = value;

			value = value/200+1;

			colIndex = index*4;
			colorArray[colIndex] = value;
			colorArray[colIndex+1] = value;
			colorArray[colIndex+2] = value;
			colorArray[colIndex+3] = value;
		}
	}
}

void WaterWaveAction::RippleSpread()
{
	float *vertArray1 = (float*)buff1;
	float *vertArray2 = (float*)buff2;
    int i;
    for (i = 0; i < _grid_count; ++i)
    {
		float temp = 0;
		if (i>0) {
			temp += vertArray1[i-1];
			if (i>=_bo_length) {
				temp += vertArray1[i-_bo_length];
			}
		}
		
		if (i<_grid_count-1) {
			temp += vertArray1[i+1];
			if (i<_grid_count-_bo_length) {
				temp += vertArray1[i+_bo_length];
			}
		}
		float value = temp / 2 - vertArray2[i];
		value -= value/100;
		if (value > 15) value = 15;
		if (value < -15) value = -15;
		vertArray2[i] = value;
    }

	//交换波能数据缓冲区     
	GLvoid *ptmp =buff1;
	buff1 = buff2;
	buff2 = ptmp; 
}


void WaterWaveAction::dropStone(int x, int y, float value)
{

	float *buff = (float*)buff2;
    int i, j;
	int r = 5;
	for(i=-r; i<r; i++)
	{
		for(j=-r; j<r; j++)
		{
			if((i*i + j*j) <= r*r)
			{
				int x1 = x+i;
				int y1= y+j;
				int index = x1*_bo_length + y1;
				if (x1<0 || y1 < 0 || x1>_gridSize.width || y1 > _gridSize.height)
				{
				}
				else
				{
					buff[index] = value;
					//setBuff(x+i, y+j, buff, value);
					log("i=%d, j=%d", i, j);
				}
			}
		}
	}
}

void WaterWaveAction::setColor(const Point& position, const Color4F& color)
{
    GridColor3D *g = (GridColor3D*)_target->getGrid();
	g->setGridColor(position, color);
}

GridBase* WaterWaveAction::getGrid()
{
    return GridColor3D::create(_gridSize);
}

GLvoid* GridColor3D::getOriginalVertex()
{
	return _vertices;
}

GridColor3D* GridColor3D::create(const Size& gridSize, Texture2D *texture, bool bFlipped)
{
    GridColor3D *pRet= new GridColor3D();

    if (pRet)
    {
        if (pRet->initWithSize(gridSize, texture, bFlipped))
        {
            pRet->autorelease();
        }
        else
        {
            delete pRet;
            pRet = NULL;
        }
    }

    return pRet;
}

GridColor3D* GridColor3D::create(const Size& gridSize)
{
    GridColor3D *pRet= new GridColor3D();

    if (pRet)
    {
        if (pRet->initWithSize(gridSize))
        {
            pRet->autorelease();
        }
        else
        {
            delete pRet;
            pRet = NULL;
        }
    }

    return pRet;
}


GridColor3D::GridColor3D()
    :  _colors(NULL)
{

}

GridColor3D::~GridColor3D(void)
{
	CC_SAFE_FREE(_colors);
}

void GridColor3D::blit(void)
{
    int n = _gridSize.width * _gridSize.height;

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    _shaderProgram->use();
    _shaderProgram->setUniformsForBuiltins();

    //
    // Attributes
    //
#ifdef EMSCRIPTEN
    // Size calculations from calculateVertexPoints().
    unsigned int numOfPoints = (_gridSize.width+1) * (_gridSize.height+1);

    // position
    setGLBufferData(_vertices, numOfPoints * sizeof(Vertex3F), 0);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // texCoords
    setGLBufferData(_texCoordinates, numOfPoints * sizeof(Vertex2F), 1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // colors
	//setGLBufferData(_colors, numOfPoints * sizeof(Color4F), 1);
 //   glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 4, GL_FLOAT, GL_TRUE, 0, 0);

    setGLIndexData(_indices, n * 12, 0);
    glDrawElements(GL_TRIANGLES, (GLsizei) n*6, GL_UNSIGNED_SHORT, 0);
#else
    // position
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, _vertices);

    // texCoords
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, _texCoordinates);
	
	// colors
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, _colors);


    glDrawElements(GL_TRIANGLES, (GLsizei) n*6, GL_UNSIGNED_SHORT, _indices);
#endif // EMSCRIPTEN
    CC_INCREMENT_GL_DRAWS(1);
}

void GridColor3D::calculateVertexPoints(void)
{
	_shaderProgram = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
    float width = (float)_texture->getPixelsWide();
    float height = (float)_texture->getPixelsHigh();
    float imageH = _texture->getContentSizeInPixels().height;

    int x, y, i;
	
    CC_SAFE_FREE(_colors);
    CC_SAFE_FREE(_vertices);
    CC_SAFE_FREE(_originalVertices);
    CC_SAFE_FREE(_texCoordinates);
    CC_SAFE_FREE(_indices);

    unsigned int numOfPoints = (_gridSize.width+1) * (_gridSize.height+1);

    _vertices = malloc(numOfPoints * sizeof(Vertex3F));
    _originalVertices = malloc(numOfPoints * sizeof(Vertex3F));
    _texCoordinates = malloc(numOfPoints * sizeof(Vertex2F));
	_colors = malloc(numOfPoints * sizeof(float)*4);
    _indices = (GLushort*)malloc(_gridSize.width * _gridSize.height * sizeof(GLushort) * 6);

    GLfloat *vertArray = (GLfloat*)_vertices;
    GLfloat *texArray = (GLfloat*)_texCoordinates;
	GLfloat *colorArray = (GLfloat*)_colors;
    GLushort *idxArray = _indices;

    for (x = 0; x < _gridSize.width; ++x)
    {
        for (y = 0; y < _gridSize.height; ++y)
        {
            int idx = (y * _gridSize.width) + x;

            GLfloat x1 = x * _step.x;
            GLfloat x2 = x1 + _step.x;
            GLfloat y1 = y * _step.y;
            GLfloat y2= y1 + _step.y;

            GLushort a = (GLushort)(x * (_gridSize.height + 1) + y);
            GLushort b = (GLushort)((x + 1) * (_gridSize.height + 1) + y);
            GLushort c = (GLushort)((x + 1) * (_gridSize.height + 1) + (y + 1));
            GLushort d = (GLushort)(x * (_gridSize.height + 1) + (y + 1));

            GLushort tempidx[6] = {a, b, d, b, c, d};

            memcpy(&idxArray[6*idx], tempidx, 6*sizeof(GLushort));

            int l1[4] = {a*3, b*3, c*3, d*3};
            Vertex3F e(x1, y1, 0);
            Vertex3F f(x2, y1, 0);
            Vertex3F g(x2, y2, 0);
            Vertex3F h(x1, y2, 0);

            Vertex3F l2[4] = {e, f, g, h};

            int tex1[4] = {a*2, b*2, c*2, d*2};
            Point Tex2F[4] = {Point(x1, y1), Point(x2, y1), Point(x2, y2), Point(x1, y2)};

            int color1[4] = {a*4, b*4, c*4, d*4};

            for (i = 0; i < 4; ++i)
            {
                vertArray[l1[i]] = l2[i].x;
                vertArray[l1[i] + 1] = l2[i].y;
                vertArray[l1[i] + 2] = l2[i].z;

                texArray[tex1[i]] = Tex2F[i].x / width;
                if (_isTextureFlipped)
                {
                    texArray[tex1[i] + 1] = (imageH - Tex2F[i].y) / height;
                }
                else
                {
                    texArray[tex1[i] + 1] = Tex2F[i].y / height;
                }
				
				colorArray[color1[i]] = 1;
				colorArray[color1[i]+1] = 1;
				colorArray[color1[i]+2] = 1;
				colorArray[color1[i]+3] = 1;
            }
        }
    }

    memcpy(_originalVertices, _vertices, (_gridSize.width+1) * (_gridSize.height+1) * sizeof(Vertex3F));

}

Color4F GridColor3D::getGridColor(const Point& pos) const
{
    CCASSERT( pos.x == (unsigned int)pos.x && pos.y == (unsigned int) pos.y , "Numbers must be integers");
    
    int index = (pos.x * (_gridSize.height+1) + pos.y) * 4;
	float *colorArray = (float*)_colors;

	return Color4F(colorArray[index], colorArray[index+1], colorArray[index+2], colorArray[index+3]);
}

GLvoid* GridColor3D::getGridColor() const
{
	return _colors;
}

void GridColor3D::setGridColor(const Point& pos, const Color4F& color)
{
    CCASSERT( pos.x == (unsigned int)pos.x && pos.y == (unsigned int) pos.y , "Numbers must be integers");
    int index = (pos.x * (_gridSize.height + 1) + pos.y) * 4;
    float *colorArray = (float*)_colors;
	colorArray[index] = color.r;
    colorArray[index+1] = color.g;
    colorArray[index+2] = color.b;
    colorArray[index+3] = color.a;
}

void GridColor3D::reuse(void)
{
    if (_reuseGrid > 0)
    {
        memcpy(_originalVertices, _vertices, (_gridSize.width+1) * (_gridSize.height+1) * sizeof(Vertex3F));
        --_reuseGrid;
    }
}