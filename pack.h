#ifndef INC_PACK_H
/// 
#define INC_PACK_H

/** @addtogroup S644x679_P239x248
 * @{
 */
// [p239#644]
#define exactly         0
#define additional      1
// [p250#681]
#define mathchar        1
#define subbox          2
#define submlist        3
#define mathtextchar    4
/** @}*/ // end group S644x679_P239x248


extern Scaled totalstretch[FILLL - NORMAL + 1], totalshrink[FILLL - NORMAL + 1];
extern Integer lastbadness;
extern Pointer adjusttail;
extern Integer packbeginline;

extern void pack_init();
extern void scanspec(GroupCode c, Boolean threecodes);
extern HalfWord hpack(HalfWord p, long w, SmallNumber m);
extern HalfWord vpackage(HalfWord p, long h, SmallNumber m, long l);
extern void appendtovlist(HalfWord b);

#endif /* INC_PACK_H */