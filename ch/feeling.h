#ifndef FEELING_H
#define FEELING_H
#include<algorithm>
#define randf() (float)rand()/(float)RAND_MAX

namespace hax
{
    struct Distribution
    {
        float p[4];
        void normalize()
        {
            float n = 0.0f;
            for(int i = 0;i<4;i++)
            {
                n+=std::max(p[i],0.0f);    
            }
            for(int i = 0;i<4;i++)
            {
                p[i]/=n;
            }
        };
        int sample() const
        {
            float r = randf();
            float acc = 0.0f;
            int i = 0;
            while(acc<r)
            {
                acc += p[i];
                ++i;
            }
            return i;
        };
    };
    
    class Character;
    class Feeling
    {
      public:
        Feeling(const Distribution);
        bool(Character::*)(void) getRandomAction() const;
      private:
        const Distribution dist;
        std::vector<bool(Character::*)(void)> actions;
    };
}
#endif
