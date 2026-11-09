// A fragment of EA's MAD movie codec (rcmp_mad_codec.cpp): handing out frames. Frames sit on a free list and, while in
// use, on a used list, linked through a node inside the frame. Names come from the symbol table; members are inferred from
// the code. The class is virtual in the original; this view does not need the table.
#include "Rcmp.h"
extern "C" void REAL_abortmessage(const char *, ...);

struct FrameLink {
    FrameLink *next;
    FrameLink *previous;
};
class MAD_FRAME {
  public:
    unsigned char unknown00[8];
    FrameLink link; // +0x08
    int refs;       // +0x10
};
// A removed link's words are overwritten with 11, as a marker.
static inline void AddTail(FrameLink *list, FrameLink *link) {
    FrameLink *last = list->previous;
    last->next = link;
    list->previous = link;
    link->previous = last;
    link->next = list;
}

class MAD_CODEC_INTERNAL {
  public:
    MAD_FRAME *CreateKorM();
    MAD_FRAME *CreateE();
    void ReleaseFrame(RCMP::FRAME *);
    unsigned char unknown00[0x20];
    FrameLink used; // +0x20
    FrameLink free; // +0x28
};

static inline MAD_FRAME *FrameOf(FrameLink *link) { return link ? (MAD_FRAME *)((char *)link - 8) : 0; }

MAD_FRAME *MAD_CODEC_INTERNAL::CreateKorM() {
    if (free.next != &free) {
        FrameLink *link = free.next;
        FrameLink *next = link->next;
        FrameLink *previous = link->previous;
        previous->next = next;
        next->previous = previous;
        link->next = (FrameLink *)11;
        link->previous = (FrameLink *)11;
        MAD_FRAME *frame = FrameOf(link);
        AddTail(&used, frame ? &frame->link : 0);
        frame->refs = 2;
        return frame;
    }
    REAL_abortmessage("MAD_CODEC_INTERNAL::CreateIorP() out of frames did you call CODEC::ReleaseFrame( FRAME *Frame )\n");
    return 0;
}

MAD_FRAME *MAD_CODEC_INTERNAL::CreateE() {
    if (free.next != &free) {
        FrameLink *link = free.next;
        FrameLink *next = link->next;
        FrameLink *previous = link->previous;
        previous->next = next;
        next->previous = previous;
        link->next = (FrameLink *)11;
        link->previous = (FrameLink *)11;
        MAD_FRAME *frame = FrameOf(link);
        AddTail(&used, frame ? &frame->link : 0);
        frame->refs = 1;
        return frame;
    }
    REAL_abortmessage("MAD_CODEC_INTERNAL::CreateB() out of frames did you call CODEC::ReleaseFrame( FRAME *Frame )\n");
    return 0;
}

void MAD_CODEC_INTERNAL::ReleaseFrame(RCMP::FRAME *released) {
    MAD_FRAME *frame = (MAD_FRAME *)released;
    frame->refs = frame->refs - 1;
    if (frame->refs > 0)
        return;
    FrameLink *link = &frame->link;
    FrameLink *next = link->next;
    FrameLink *previous = link->previous;
    previous->next = next;
    next->previous = previous;
    link->next = (FrameLink *)11;
    link->previous = (FrameLink *)11;
    AddTail(&free, frame ? link : 0);
}
