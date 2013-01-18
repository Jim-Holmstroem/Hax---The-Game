#ifndef FEELING_H
#define FEELING_H

#include<algorithm>
#define randf() (float)rand()/(float)RAND_MAX

namespace hax
{
    class Character;
    template<int N>
    struct Distribution
    {
        float p[N];
        void normalize()
        {
            float n = 0.0f;
            for(int i = 0;i<N;i++)
            {
                n+=max(p[i],0.0f);    
            }
            for(int i = 0;i<N;i++)
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
    class Feeling
    {
      public:
        Feeling(const Distribution<4>);
        bool(Character::*)(void) getRandomAction() const;
      private:
        const Distribution<4> dist;
        std::vector<bool(Character::*)(void)> actions;
    };
}
#endif
