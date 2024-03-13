#ifndef TACHECOMPOSITE_H
#define TACHECOMPOSITE_H

#include "tache.h"

class TacheComposite : public Tache
{
public:
    explicit TacheComposite(QObject *parent = nullptr);
};

#endif // TACHECOMPOSITE_H
