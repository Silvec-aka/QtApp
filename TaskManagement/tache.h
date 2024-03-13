#ifndef TACHE_H
#define TACHE_H

#include <QObject>

class Tache : public QObject
{
    Q_OBJECT
public:
    explicit Tache(QObject *parent = nullptr);

signals:
};

#endif // TACHE_H
