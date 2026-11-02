// Fragment of box.cpp: squared distances from a point, a line and a segment to a box, with
// the line's local case functions.
#include "Box.h"
#include "CLine3.h"
#include "CMatrix.h"

// Component access by index. The const form's integer sum reproduces the operand order of the
// original's indexed loads (base register second); it reads the same element. Names follow the
// standard point/line-box distance formulation and are descriptive.
static inline float &At(CVector3 &v, int i) { return (&v.x)[i]; }
static inline const float &At(const CVector3 &v, int i) { return *(const float *)(i * 4 + (unsigned int)&v); }
static inline void BoxExtent(const CVolBox &box, int i, float &extent) {
    switch (i) {
    case 0:
        extent = box.halfSize[0];
        break;
    case 1:
        extent = box.halfSize[1];
        break;
    case 2:
        extent = box.halfSize[2];
        break;
    }
}

static inline CVector3 StartCopy(const CLine3 &line) { return CVector3(line.start); }
static inline CVector3 EndCopy(const CLine3 &line) { return CVector3(line.end); }
static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
static inline void TransformPoint(CVector3 &out, const CMatrix &m, CVector3 p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}
static inline void TransformPointRef(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}
static inline void TransformVector(CVector3 &out, const CMatrix &m, CVector3 p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z;
    out = r;
}

static void Case0(int, int, int, CVector3 &, const CVector3 &, const CVolBox &, float *, float &);
static void Case00(int, int, int, CVector3 &, const CVector3 &, const CVolBox &, float *, float &);
static void Face(int, int, int, CVector3 &, const CVector3 &, const CVolBox &, const CVector3 &, float *, float &);
static void CaseNoZeros(CVector3 &, const CVector3 &, const CVolBox &, float *, float &);
static void Case000(CVector3 &, const CVolBox &, float &);

float BoxPointSqrDistance(const CVector3 &rkPoint, const CVolBox &rkBox, float *pfBParam0, float *pfBParam1, float *pfBParam2) {
    CMatrix kMat;
    CMatrix *pMat = &kMat;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    kMat.GetSlot(0);
    kMat.SetRight(CVector3(rkBox.axisX));
    kMat.SetFront(CVector3(rkBox.axisY));
    kMat.SetUp(CVector3(rkBox.axisZ));
    kMat.SetPos(CVector3(rkBox.center));
    CMatrix::s_TempMat = kMat;
    kMat.FastInverse(CMatrix::s_TempMat);
    CVector3 kClosest;
    TransformPointRef(kClosest, *pMat, rkPoint);
    float fSqrDistance = 0.0f;
    float fDelta;
    float fE0 = rkBox.halfSize[0];
    if (!(kClosest.x >= -fE0)) {
        fDelta = kClosest.x + fE0;
        fSqrDistance = fDelta * fDelta;
        kClosest.x = -fE0;
    } else if (!(kClosest.x <= fE0)) {
        fDelta = kClosest.x - fE0;
        fSqrDistance = fDelta * fDelta;
        kClosest.x = fE0;
    }
    float fE1 = rkBox.halfSize[1];
    if (!(kClosest.y >= -fE1)) {
        fDelta = kClosest.y + fE1;
        fSqrDistance += fDelta * fDelta;
        kClosest.y = -fE1;
    } else if (!(kClosest.y <= fE1)) {
        fDelta = kClosest.y - fE1;
        fSqrDistance += fDelta * fDelta;
        kClosest.y = fE1;
    }
    float fE2 = rkBox.halfSize[2];
    if (!(kClosest.z >= -fE2)) {
        fDelta = kClosest.z + fE2;
        fSqrDistance += fDelta * fDelta;
        kClosest.z = -fE2;
    } else if (!(kClosest.z <= fE2)) {
        fDelta = kClosest.z - fE2;
        fSqrDistance += fDelta * fDelta;
        kClosest.z = fE2;
    }
    if (pfBParam0) {
        *pfBParam0 = kClosest.x;
        *pfBParam1 = kClosest.y;
        *pfBParam2 = kClosest.z;
    }
    return fSqrDistance;
}

static void Face(int i0, int i1, int i2, CVector3& rkPnt, const CVector3& rkDir, const CVolBox& rkBox, const CVector3& rkPmE, float* pfLParam, float& rfSqrDistance) {
    CVector3 kPpE;
float fE1 = 0.0f, fE0 = 0.0f, fE2 = 0.0f;
    BoxExtent(rkBox, i0, fE0);
    BoxExtent(rkBox, i1, fE1);
    BoxExtent(rkBox, i2, fE2);
    float fLSqr, fInv, fTmp, fParam, fT, fDelta;

    At(kPpE, i1) = At(rkPnt, i1) + fE1;
    At(kPpE, i2) = At(rkPnt, i2) + fE2;
    if (!(At(rkDir, i0) * At(kPpE, i1) < At(rkDir, i1) * At(rkPmE, i0))) {
        if (!(At(rkDir, i0) * At(kPpE, i2) < At(rkDir, i2) * At(rkPmE, i0))) {
            if (pfLParam) {
                At(rkPnt, i0) = fE0;
                fInv = 1.0f / At(rkDir, i0);
                At(rkPnt, i1) -= At(rkDir, i1) * At(rkPmE, i0) * fInv;
                At(rkPnt, i2) -= At(rkDir, i2) * At(rkPmE, i0) * fInv;
                *pfLParam = -At(rkPmE, i0) * fInv;
            }
        } else {
            fLSqr = At(rkDir, i0) * At(rkDir, i0) + At(rkDir, i2) * At(rkDir, i2);
            fTmp = fLSqr * At(kPpE, i1) - At(rkDir, i1) * (At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i2) * At(kPpE, i2));
            if (!(fTmp > 2.0f * fLSqr * fE1)) {
                fT = fTmp / fLSqr;
                fLSqr += At(rkDir, i1) * At(rkDir, i1);
                fTmp = At(kPpE, i1) - fT;
                fDelta = At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i1) * fTmp + At(rkDir, i2) * At(kPpE, i2);
                fParam = -fDelta / fLSqr;
                rfSqrDistance += At(rkPmE, i0) * At(rkPmE, i0) + fTmp * fTmp + At(kPpE, i2) * At(kPpE, i2) + fDelta * fParam;
                if (pfLParam) {
                    *pfLParam = fParam;
                    At(rkPnt, i0) = fE0;
                    At(rkPnt, i1) = fT - fE1;
                    At(rkPnt, i2) = -fE2;
                }
            } else {
                fLSqr += At(rkDir, i1) * At(rkDir, i1);
                fDelta = At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i1) * At(rkPmE, i1) + At(rkDir, i2) * At(kPpE, i2);
                fParam = -fDelta / fLSqr;
                rfSqrDistance += At(rkPmE, i0) * At(rkPmE, i0) + At(rkPmE, i1) * At(rkPmE, i1) + At(kPpE, i2) * At(kPpE, i2) + fDelta * fParam;
                if (pfLParam) {
                    *pfLParam = fParam;
                    At(rkPnt, i0) = fE0;
                    At(rkPnt, i1) = fE1;
                    At(rkPnt, i2) = -fE2;
                }
            }
        }
    } else {
        if (!(At(rkDir, i0) * At(kPpE, i2) < At(rkDir, i2) * At(rkPmE, i0))) {
            fLSqr = At(rkDir, i0) * At(rkDir, i0) + At(rkDir, i1) * At(rkDir, i1);
            fTmp = fLSqr * At(kPpE, i2) - At(rkDir, i2) * (At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i1) * At(kPpE, i1));
            if (!(fTmp > 2.0f * fLSqr * fE2)) {
                fT = fTmp / fLSqr;
                fLSqr += At(rkDir, i2) * At(rkDir, i2);
                fTmp = At(kPpE, i2) - fT;
                fDelta = At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i1) * At(kPpE, i1) + At(rkDir, i2) * fTmp;
                fParam = -fDelta / fLSqr;
                rfSqrDistance += At(rkPmE, i0) * At(rkPmE, i0) + At(kPpE, i1) * At(kPpE, i1) + fTmp * fTmp + fDelta * fParam;
                if (pfLParam) {
                    *pfLParam = fParam;
                    At(rkPnt, i0) = fE0;
                    At(rkPnt, i1) = -fE1;
                    At(rkPnt, i2) = fT - fE2;
                }
            } else {
                fLSqr += At(rkDir, i2) * At(rkDir, i2);
                fDelta = At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i1) * At(kPpE, i1) + At(rkDir, i2) * At(rkPmE, i2);
                fParam = -fDelta / fLSqr;
                rfSqrDistance += At(rkPmE, i0) * At(rkPmE, i0) + At(kPpE, i1) * At(kPpE, i1) + At(rkPmE, i2) * At(rkPmE, i2) + fDelta * fParam;
                if (pfLParam) {
                    *pfLParam = fParam;
                    At(rkPnt, i0) = fE0;
                    At(rkPnt, i1) = -fE1;
                    At(rkPnt, i2) = fE2;
                }
            }
        } else {
            fLSqr = At(rkDir, i0) * At(rkDir, i0) + At(rkDir, i2) * At(rkDir, i2);
            fTmp = fLSqr * At(kPpE, i1) - At(rkDir, i1) * (At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i2) * At(kPpE, i2));
            if (!(fTmp < 0.0f)) {
                if (!(fTmp > 2.0f * fLSqr * fE1)) {
                    fT = fTmp / fLSqr;
                    fLSqr += At(rkDir, i1) * At(rkDir, i1);
                    fTmp = At(kPpE, i1) - fT;
                    fDelta = At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i1) * fTmp + At(rkDir, i2) * At(kPpE, i2);
                    fParam = -fDelta / fLSqr;
                    rfSqrDistance += At(rkPmE, i0) * At(rkPmE, i0) + fTmp * fTmp + At(kPpE, i2) * At(kPpE, i2) + fDelta * fParam;
                    if (pfLParam) {
                        *pfLParam = fParam;
                        At(rkPnt, i0) = fE0;
                        At(rkPnt, i1) = fT - fE1;
                        At(rkPnt, i2) = -fE2;
                    }
                } else {
                    fLSqr += At(rkDir, i1) * At(rkDir, i1);
                    fDelta = At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i1) * At(rkPmE, i1) + At(rkDir, i2) * At(kPpE, i2);
                    fParam = -fDelta / fLSqr;
                    rfSqrDistance += At(rkPmE, i0) * At(rkPmE, i0) + At(rkPmE, i1) * At(rkPmE, i1) + At(kPpE, i2) * At(kPpE, i2) + fDelta * fParam;
                    if (pfLParam) {
                        *pfLParam = fParam;
                        At(rkPnt, i0) = fE0;
                        At(rkPnt, i1) = fE1;
                        At(rkPnt, i2) = -fE2;
                    }
                }
                return;
            }

            fLSqr = At(rkDir, i0) * At(rkDir, i0) + At(rkDir, i1) * At(rkDir, i1);
            fTmp = fLSqr * At(kPpE, i2) - At(rkDir, i2) * (At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i1) * At(kPpE, i1));
            if (!(fTmp < 0.0f)) {
                if (!(fTmp > 2.0f * fLSqr * fE2)) {
                    fT = fTmp / fLSqr;
                    fLSqr += At(rkDir, i2) * At(rkDir, i2);
                    fTmp = At(kPpE, i2) - fT;
                    fDelta = At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i1) * At(kPpE, i1) + At(rkDir, i2) * fTmp;
                    fParam = -fDelta / fLSqr;
                    rfSqrDistance += At(rkPmE, i0) * At(rkPmE, i0) + At(kPpE, i1) * At(kPpE, i1) + fTmp * fTmp + fDelta * fParam;
                    if (pfLParam) {
                        *pfLParam = fParam;
                        At(rkPnt, i0) = fE0;
                        At(rkPnt, i1) = -fE1;
                        At(rkPnt, i2) = fT - fE2;
                    }
                } else {
                    fLSqr += At(rkDir, i2) * At(rkDir, i2);
                    fDelta = At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i1) * At(kPpE, i1) + At(rkDir, i2) * At(rkPmE, i2);
                    fParam = -fDelta / fLSqr;
                    rfSqrDistance += At(rkPmE, i0) * At(rkPmE, i0) + At(kPpE, i1) * At(kPpE, i1) + At(rkPmE, i2) * At(rkPmE, i2) + fDelta * fParam;
                    if (pfLParam) {
                        *pfLParam = fParam;
                        At(rkPnt, i0) = fE0;
                        At(rkPnt, i1) = -fE1;
                        At(rkPnt, i2) = fE2;
                    }
                }
                return;
            }

            fLSqr += At(rkDir, i2) * At(rkDir, i2);
            fDelta = At(rkDir, i0) * At(rkPmE, i0) + At(rkDir, i1) * At(kPpE, i1) + At(rkDir, i2) * At(kPpE, i2);
            fParam = -fDelta / fLSqr;
            rfSqrDistance += At(rkPmE, i0) * At(rkPmE, i0) + At(kPpE, i1) * At(kPpE, i1) + At(kPpE, i2) * At(kPpE, i2) + fDelta * fParam;
            if (pfLParam) {
                *pfLParam = fParam;
                At(rkPnt, i0) = fE0;
                At(rkPnt, i1) = -fE1;
                At(rkPnt, i2) = -fE2;
            }
        }
    }
}

static void CaseNoZeros(CVector3 &rkPnt, const CVector3 &rkDir, const CVolBox &rkBox, float *pfLParam, float &rfSqrDistance) {
    CVector3 kExtent(rkBox.halfSize[0], rkBox.halfSize[1], rkBox.halfSize[2]);
    CVector3 kPmE;
    SetSub(kPmE, rkPnt, kExtent);
    // Plain reference: reproduces the code shape.
    const CVector3 &kRef = kPmE;

    float fProdDxPy = rkDir.x * kPmE.y;
    float fProdDyPx = rkDir.y * kPmE.x;
    if (!(fProdDyPx < fProdDxPy)) {
        float fProdDzPx = rkDir.z * kPmE.x;
        float fProdDxPz = rkDir.x * kPmE.z;
        if (!(fProdDzPx < fProdDxPz))
            Face(0, 1, 2, rkPnt, rkDir, rkBox, kRef, pfLParam, rfSqrDistance);
        else
            Face(2, 0, 1, rkPnt, rkDir, rkBox, kRef, pfLParam, rfSqrDistance);
    } else {
        float fProdDzPy = rkDir.z * kPmE.y;
        float fProdDyPz = rkDir.y * kPmE.z;
        if (!(fProdDzPy < fProdDyPz))
            Face(1, 2, 0, rkPnt, rkDir, rkBox, kRef, pfLParam, rfSqrDistance);
        else
            Face(2, 0, 1, rkPnt, rkDir, rkBox, kRef, pfLParam, rfSqrDistance);
    }
}

static void Case0(int i0, int i1, int i2, CVector3& rkPnt, const CVector3& rkDir, const CVolBox& rkBox, float* pfLParam, float& rfSqrDistance) {
    float fE0 = 0.0f;
    float fE1 = fE0;
    float fE2 = fE0;
    BoxExtent(rkBox, i0, fE0);
    BoxExtent(rkBox, i1, fE1);
    BoxExtent(rkBox, i2, fE2);
    float fPmE0 = At(rkPnt, i0) - fE0;
    float fPmE1 = At(rkPnt, i1) - fE1;
    float fProd0 = At(rkDir, i1) * fPmE0;
    float fProd1 = At(rkDir, i0) * fPmE1;
    float fDelta, fInvLSqr, fInv;

    if (!(fProd0 < fProd1)) {
        At(rkPnt, i0) = fE0;

        float fPpE1 = At(rkPnt, i1) + fE1;
        fDelta = fProd0 - At(rkDir, i0) * fPpE1;
        if (!(fDelta < 0.0f)) {
            fInvLSqr = 1.0f / (At(rkDir, i0) * At(rkDir, i0) + At(rkDir, i1) * At(rkDir, i1));
            rfSqrDistance += fDelta * fDelta * fInvLSqr;
            if (pfLParam) {
                At(rkPnt, i1) = -fE1;
                *pfLParam = -(At(rkDir, i0) * fPmE0 + At(rkDir, i1) * fPpE1) * fInvLSqr;
            }
        } else {
            if (pfLParam) {
                fInv = 1.0f / At(rkDir, i0);
                At(rkPnt, i1) -= fProd0 * fInv;
                *pfLParam = -fPmE0 * fInv;
            }
        }
    } else {
        At(rkPnt, i1) = fE1;

        float fPpE0 = At(rkPnt, i0) + fE0;
        fDelta = fProd1 - At(rkDir, i1) * fPpE0;
        if (!(fDelta < 0.0f)) {
            fInvLSqr = 1.0f / (At(rkDir, i0) * At(rkDir, i0) + At(rkDir, i1) * At(rkDir, i1));
            rfSqrDistance += fDelta * fDelta * fInvLSqr;
            if (pfLParam) {
                At(rkPnt, i0) = -fE0;
                *pfLParam = -(At(rkDir, i0) * fPpE0 + At(rkDir, i1) * fPmE1) * fInvLSqr;
            }
        } else {
            if (pfLParam) {
                fInv = 1.0f / At(rkDir, i1);
                At(rkPnt, i0) -= fProd1 * fInv;
                *pfLParam = -fPmE1 * fInv;
            }
        }
    }

    if (!(At(rkPnt, i2) >= -fE2)) {
        fDelta = At(rkPnt, i2) + fE2;
        rfSqrDistance += fDelta * fDelta;
        At(rkPnt, i2) = -fE2;
    } else if (!(At(rkPnt, i2) <= fE2)) {
        fDelta = At(rkPnt, i2) - fE2;
        rfSqrDistance += fDelta * fDelta;
        At(rkPnt, i2) = fE2;
    }
}

static void Case00(int i0, int i1, int i2, CVector3& rkPnt, const CVector3& rkDir, const CVolBox& rkBox, float* pfLParam, float& rfSqrDistance) {
    float fE1 = 0.0f, fE0 = 0.0f, fE2 = 0.0f;
    BoxExtent(rkBox, i0, fE0);
    BoxExtent(rkBox, i1, fE1);
    BoxExtent(rkBox, i2, fE2);
    float fDelta;

    if (pfLParam)
        *pfLParam = (fE0 - At(rkPnt, i0)) / At(rkDir, i0);

    At(rkPnt, i0) = fE0;

    if (!(At(rkPnt, i1) >= -fE1)) {
        fDelta = At(rkPnt, i1) + fE1;
        rfSqrDistance += fDelta * fDelta;
        At(rkPnt, i1) = -fE1;
    } else if (!(At(rkPnt, i1) <= fE1)) {
        fDelta = At(rkPnt, i1) - fE1;
        rfSqrDistance += fDelta * fDelta;
        At(rkPnt, i1) = fE1;
    }

    if (!(At(rkPnt, i2) >= -fE2)) {
        fDelta = At(rkPnt, i2) + fE2;
        rfSqrDistance += fDelta * fDelta;
        At(rkPnt, i2) = -fE2;
    } else if (!(At(rkPnt, i2) <= fE2)) {
        fDelta = At(rkPnt, i2) - fE2;
        rfSqrDistance += fDelta * fDelta;
        At(rkPnt, i2) = fE2;
    }
}

static void Case000(CVector3 &rkPnt, const CVolBox &rkBox, float &rfSqrDistance) {
    float fDelta;
    if (!(rkPnt.x >= -rkBox.halfSize[0])) {
        fDelta = rkPnt.x + rkBox.halfSize[0];
        rfSqrDistance += fDelta * fDelta;
        rkPnt.x = -rkBox.halfSize[0];
    } else if (!(rkPnt.x <= rkBox.halfSize[0])) {
        fDelta = rkPnt.x - rkBox.halfSize[0];
        rfSqrDistance += fDelta * fDelta;
        rkPnt.x = rkBox.halfSize[0];
    }
    if (!(rkPnt.y >= -rkBox.halfSize[1])) {
        fDelta = rkPnt.y + rkBox.halfSize[1];
        rfSqrDistance += fDelta * fDelta;
        rkPnt.y = -rkBox.halfSize[1];
    } else if (!(rkPnt.y <= rkBox.halfSize[1])) {
        fDelta = rkPnt.y - rkBox.halfSize[1];
        rfSqrDistance += fDelta * fDelta;
        rkPnt.y = rkBox.halfSize[1];
    }
    if (!(rkPnt.z >= -rkBox.halfSize[2])) {
        fDelta = rkPnt.z + rkBox.halfSize[2];
        rfSqrDistance += fDelta * fDelta;
        rkPnt.z = -rkBox.halfSize[2];
    } else if (!(rkPnt.z <= rkBox.halfSize[2])) {
        fDelta = rkPnt.z - rkBox.halfSize[2];
        rfSqrDistance += fDelta * fDelta;
        rkPnt.z = rkBox.halfSize[2];
    }
}

float BoxLineSqrDistance(const CLine3 &rkLine, const CVolBox &rkBox, float *pfLParam, float *pfBParam0, float *pfBParam1, float *pfBParam2) {
    CMatrix kMat;
    CMatrix *pMat = &kMat;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    kMat.GetSlot(0);
    kMat.SetRight(CVector3(rkBox.axisX));
    kMat.SetFront(CVector3(rkBox.axisY));
    kMat.SetUp(CVector3(rkBox.axisZ));
    kMat.SetPos(CVector3(rkBox.center));
    CMatrix::s_TempMat = kMat;
    kMat.FastInverse(CMatrix::s_TempMat);
    CVector3 kPnt;
    TransformPoint(kPnt, *pMat, rkLine.start);
    CVector3 kDir;
    TransformVector(kDir, *pMat, rkLine.direction);
    int bReflect[3];
    int i;
    for (i = 0; i < 3; i++) {
        bReflect[i] = At(kDir, i) < 0.0f;
        if (bReflect[i]) {
            At(kPnt, i) = -At(kPnt, i);
            At(kDir, i) = -At(kDir, i);
        }
    }
    float fSqrDistance = 0.0f;
    if (!(kDir.x <= 1.880790961e-37f)) {
        if (!(kDir.y <= 1.880790961e-37f)) {
            if (!(kDir.z <= 1.880790961e-37f))
                CaseNoZeros(kPnt, kDir, rkBox, pfLParam, fSqrDistance);
            else
                Case0(0, 1, 2, kPnt, kDir, rkBox, pfLParam, fSqrDistance);
        } else {
            if (!(kDir.z <= 1.880790961e-37f))
                Case0(0, 2, 1, kPnt, kDir, rkBox, pfLParam, fSqrDistance);
            else
                Case00(0, 1, 2, kPnt, kDir, rkBox, pfLParam, fSqrDistance);
        }
    } else {
        if (!(kDir.y <= 1.880790961e-37f)) {
            if (!(kDir.z <= 1.880790961e-37f))
                Case0(1, 2, 0, kPnt, kDir, rkBox, pfLParam, fSqrDistance);
            else
                Case00(1, 0, 2, kPnt, kDir, rkBox, pfLParam, fSqrDistance);
        } else {
            if (!(kDir.z <= 1.880790961e-37f)) {
                Case00(2, 0, 1, kPnt, kDir, rkBox, pfLParam, fSqrDistance);
            } else {
                Case000(kPnt, rkBox, fSqrDistance);
                if (pfLParam)
                    *pfLParam = 0.0f;
            }
        }
    }
    if (pfLParam) {
        for (i = 0; i < 3; i++) {
            if (bReflect[i])
                At(kPnt, i) = -At(kPnt, i);
        }
        *pfBParam0 = kPnt.x;
        *pfBParam1 = kPnt.y;
        *pfBParam2 = kPnt.z;
    }
    return fSqrDistance;
}

float BoxSegmentSqrDistance(const CLine3 &rkSegment, const CVolBox &rkBox, float *pfLParam, float *pfBParam0, float *pfBParam1, float *pfBParam2) {
    float fLP, fBP0, fBP1, fBP2;
    float fSqrDistance = BoxLineSqrDistance(rkSegment, rkBox, &fLP, &fBP0, &fBP1, &fBP2);
    if (!(fLP < 0.0f)) {
        if (!(fLP > 1.0f)) {
            if (pfLParam) {
                *pfLParam = fLP;
                *pfBParam0 = fBP0;
                *pfBParam1 = fBP1;
                *pfBParam2 = fBP2;
            }
        } else {
            fSqrDistance = BoxPointSqrDistance(EndCopy(rkSegment), rkBox, pfBParam0, pfBParam1, pfBParam2);
            if (pfLParam)
                *pfLParam = 1.0f;
        }
    } else {
        fSqrDistance = BoxPointSqrDistance(StartCopy(rkSegment), rkBox, pfBParam0, pfBParam1, pfBParam2);
        if (pfLParam)
            *pfLParam = 0.0f;
    }
    if (!(fSqrDistance >= 0.0f))
        fSqrDistance = 0.0f;
    return fSqrDistance;
}
