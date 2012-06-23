#ifndef MISC_H
#define MISC_H

#include <QString>
#include <cstring>
#include <cstdio>

char *fromQString(QString s) {
    const char *t = s.toStdString().c_str();
    char *str = (char *) malloc(sizeof(char) * (strlen(t) + 2));
    strcpy(str, t);
    return str;
}

#endif // MISC_H
