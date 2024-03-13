#ifndef TACHETERMINALE_H
#define TACHETERMINALE_H

#include "tache.h"

class TacheTerminale : public Tache
{
public:
    explicit TacheTerminale(QObject *parent = nullptr);
};

#endif // TACHETERMINALE_H
