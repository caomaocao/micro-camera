#ifndef JERROR_H
#define JERROR_H

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct {
  char code;
  char *msg;
} JERROR;

#ifdef __cplusplus
}
#endif 

#endif /*JERROR_H*/

