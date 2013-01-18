#ifndef FEELING_H
#define FEELING_H
#include<algorithm>

#define randf() (float)rand()/(float)RAND_MAX

namespace hax
{
    class Character;
    typedef bool(Character::*ActionPtr)(void);

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
                int j = 0;
                acc+=p[j];
                while(acc<r)
                {
                    ++j;
                    acc += p[j];
                }
                return j;
            };
    };

    class Feeling
    {
      public:
        Feeling(const Distribution);
        ActionPtr getRandomAction() const;
      private:
        Distribution dist;
        std::vector<bool(Character::*)(void)> actions;
    };


}
#endif
