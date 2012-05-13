#ifndef CENTITYMANAGER_H
#define CENTITYMANAGER_H


#include "global_inc.hpp"
#include "cEntity.hpp"



class cEntityManager
{
    public:
        cEntityManager();
        virtual ~cEntityManager();

        void ClearEntities() {
            for (int i=0; i<EntityList.size(); ++i) {
                DELETESINGLE(EntityList[i]);
            }
            EntityList.clear();
        }

        vector<cEntity*> EntityList;
    private:

};

#endif // CENTITYMANAGER_H
