// FEng's drawable objects, as FEPackageReader::CreateObject builds them. Class names and table
// symbols are from the symbol table; member names are inferred. The constructors taking a type are
// inferred from the store to FEObject::type repeated before the reader's common store.
#ifndef GAME_FENG_ELEMENTS_H
#define GAME_FENG_ELEMENTS_H
#include "FEngLists.h"
#include "FEngStrings.h"
#pragma interface

// Called when a string's label hash is read; the class name is inferred, FEString::pLabelCallback is a symbol.
class FEStringLabelCallback {
  public:
    virtual void LabelRead(class FEString *);
};
class FEString : public FEObject {
  public:
    FEString(unsigned long objectType) : unknown_58(0), labelHash(-1) {
        unknown_68 = 0;
        unknown_6c = 0;
        unknown_70 = 0;
        type = objectType;
    }
    virtual ~FEString();
    void SetLabel(const char *);
    static FEStringLabelCallback *pLabelCallback;
    unsigned long unknown_58; // +0x58
    long labelHash;           // +0x5c
    FEWideString text;        // +0x60
    unsigned long unknown_68; // +0x68
    unsigned long unknown_6c; // +0x6c
    unsigned long unknown_70; // +0x70
};
class FEImage : public FEObject {
  public:
    FEImage(unsigned long objectType) : unknown_58(0) { type = objectType; }
    virtual ~FEImage();
    unsigned long unknown_58; // +0x58
};
class FEMovie : public FEObject {
  public:
    FEMovie(unsigned long objectType) : unknown_58(0) { type = objectType; }
    virtual ~FEMovie();
    unsigned long unknown_58; // +0x58
};
class FEColoredImage : public FEObject {
  public:
    FEColoredImage(unsigned long objectType) : unknown_58(0) { type = objectType; }
    virtual ~FEColoredImage();
    unsigned long unknown_58; // +0x58
};
class FEAnimImage : public FEObject {
  public:
    FEAnimImage(unsigned long objectType) : unknown_58(0) { type = objectType; }
    virtual ~FEAnimImage();
    unsigned long unknown_58; // +0x58
};
class FESimpleImage : public FEObject {
  public:
    FESimpleImage(unsigned long objectType) { type = objectType; }
    virtual ~FESimpleImage();
};
// 196 bytes: CreateObject and Clone both allocate that much. Only the members used so far are named.
class FECodeListBox : public FEObject {
  public:
    FECodeListBox();
    virtual ~FECodeListBox();
    void FillAllCells();
    FEGameInterface *game; // +0x58
    unsigned char unknown_5c[196 - 0x5c];
};
#endif
