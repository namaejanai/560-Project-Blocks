#include<stdio.h>

#define __DEBUG_TEST__ (0) // used to test that makelist() works for all cases

const int kGridWide = 5;
const int kGridHigh = 4;
const int kEmpty = 0;
#if __DEBUG_TEST__
const int kMaxBlocks = sizeof ("ABCDEFGHI") - 1;
#else
const int kMaxBlocks = sizeof ("ABCDEFGHIJ") - 1;
#endif

enum eMove
{
    None = 0, Up, Dn, Rt, Lt
};

struct t_BLOCK
{
    int id; // 'A', 'B'. etc
    int wide, high;
    int row, col;
};

struct t_MOVELIST
{
    int id;
    eMove dir;
};

struct t_GRID
{
    int id;
};

t_GRID m_Grid[kGridHigh][kGridWide];
t_BLOCK * m_Blockset[kMaxBlocks];   // probably should use dynamic allocation
t_MOVELIST m_MoveList[100];          // should use dynamic allocation
int m_MoveListSize = 0;              // should use move list size

// put a block in the grid
void placeblock(t_BLOCK &block)
{
    int x = block.col;
    int y = block.row;
    
    for (int r = 0; r < block.high; r++)
    {
        for (int c = 0; c < block.wide; c++)
        {
            m_Grid[r + y][c + x].id = block.id;
        }
    }
}

// create a block and put it in the grid
t_BLOCK *createblock(int id, int x, int y, int w, int h)
{
    t_BLOCK *block = new t_BLOCK;
    
    block->id = id;
    block->row = y;
    block->col = x;
    block->wide = w;
    block->high = h;
    
    placeblock(*block);
    
    return block;
}

// get a pointer to a block, based on its id
t_BLOCK *findblock(int id)
{
    for (int i = 0; i < kMaxBlocks; i++)
    {
        if (m_Blockset[i]->id == id)
            return m_Blockset[i];
    }
    return 0;
}

// start with an empty grid
void cleargrid(void)
{
    for (int r = 0; r < kGridHigh; r++)
        for (int c = 0; c < kGridWide; c++)
            m_Grid[r][c].id = kEmpty;
}

// draw what's in the grid
void drawGrid(void)
{
    for (int r = 0; r < kGridHigh; r++)
    {
        for (int c = 0; c < kGridWide; c++)
        {
            int id = m_Grid[r][c].id;
            if (id == kEmpty)
                id = '-';
            printf("%c", id);
        }
        printf("\n");
    }
}

// scan the grid's blocks and make a list of possible movements
void makelist(void)
{
    int n = 0;
    
    for (int i = 0; i < kMaxBlocks; i++)
    {
        int x = m_Blockset[i]->col;
        int y = m_Blockset[i]->row;
        int w = m_Blockset[i]->wide;
        int h = m_Blockset[i]->high;
        
        // check right
        if (x + w < kGridWide)
        {
            int count = 0;
            for (int r = 0; r < h; r++)
                if (m_Grid[y + r][x + w].id == kEmpty)
                    count++;
            if (count == h)
            {
                m_MoveList[n].id = m_Blockset[i]->id;
                m_MoveList[n].dir = Rt;
                n++;
            }
        }
        
        // check left
        if (x > 0)
        {
            int count = 0;
            for (int r = 0; r < h; r++)
                if (m_Grid[y + r][x - 1].id == kEmpty)
                    count++;
            if (count == h)
            {
                m_MoveList[n].id = m_Blockset[i]->id;
                m_MoveList[n].dir = Lt;
                n++;
            }
        }
        
        // check down
        if (y + h < kGridHigh)
        {
            int count = 0;
            for (int c = 0; c < w; c++)
                if (m_Grid[y + h][x + c].id == kEmpty)
                    count++;
            if (count == w)
            {
                m_MoveList[n].id = m_Blockset[i]->id;
                m_MoveList[n].dir = Dn;
                n++;
            }
        }
        
        // check up
        if (y > 0)
        {
            int count = 0;
            for (int c = 0; c < w; c++)
                if (m_Grid[y - 1][x + c].id == kEmpty)
                    count++;
            if (count == w)
            {
                m_MoveList[n].id = m_Blockset[i]->id;
                m_MoveList[n].dir = Up;
                n++;
            }
        }
    }
    
    m_MoveListSize = n;
}

// show our list of possible movements
void showlist(void)
{
    for (int i = 0; i < m_MoveListSize; i++)
    {
        int id = m_MoveList[i].id;
        char *dir = "???";
        switch (m_MoveList[i].dir)
        {
            case Up: dir = "Up"; break;
            case Dn: dir = "Dn"; break;
            case Rt: dir = "Rt"; break;
            case Lt: dir = "Lt"; break;
        }
        int w = 0;
        int h = 0;
        int x = 0;
        int y = 0;
        t_BLOCK *b = findblock(id);
        if(b)
        {
            x = b->col;
            y = b->row;
            w = b->wide;
            h = b->high;
        }
        printf("%c at (%d,%d) [%dx%d] can move %s\n", id, x, y, w, h, dir);
    }
}

int main()
{
    cleargrid();
    
    m_Blockset[0] = createblock('A', 0, 0, 2, 1);
    m_Blockset[1] = createblock('B', 2, 0, 2, 1);
    m_Blockset[2] = createblock('C', 0, 3, 2, 1);
    m_Blockset[3] = createblock('D', 2, 3, 2, 1);
    m_Blockset[4] = createblock('E', 2, 1, 1, 2);
    m_Blockset[5] = createblock('F', 4, 0, 1, 1);
    m_Blockset[6] = createblock('G', 3, 1, 1, 1);
    m_Blockset[7] = createblock('H', 3, 2, 1, 1);
    m_Blockset[8] = createblock('I', 4, 3, 1, 1);
#if __DEBUG_TEST__ == 0
    m_Blockset[9] = createblock('J', 0, 1, 2, 2);
#endif
    
    makelist();
    drawGrid();
    showlist();
}

