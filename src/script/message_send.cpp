// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptMessageMap.h"
#include "SceneNode.h"
#include "AITargeting.h"

int BSMessageAreInSameGroup(BSObject *, BSObject *);
int BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
int BSMessageIsGroupMember(TriggerObject *, unsigned int);
int BSMessageIsGroupCategoryMember(TriggerObject *, TriggerObject *, unsigned int);
int BSMessageQueueMessage(BSObject *, BSObject *, TriggerObject *, BSMessageRegistration_struct *, void *);

int BSSendMessage(unsigned short message, BSObject *sender, TriggerObject *nativeObject, BSObject *target, void *context) {
    BSMessageEventMap::iterator it;
    it = g_message_to_event_map.find(message);
    if (it != g_message_to_event_map.end()) {
        // The original loop does not recheck end() after incrementing.
        while (it->first == message) {
            if (BSMessageAreInSameGroup(it->second.object, sender))
                BSObjectTriggerEvent(it->second.object, it->second.event, context, sender, false);
            ++it;
        }
    }

    TriggerObject *source = nativeObject;
    if (sender) source = sender->nativeObject;
    BSMessageRegistration_struct *entry = g_MessageRegistrationArray[message];
    while (entry) {
        BSObject *receiver = BSGetObjectByIndex(entry->objectIndex);
        if (!(entry->flags & 2) && receiver->queueIdentity != ~0U) {
            int matches = 0;
            BSCode_MessageHandlerEntry_struct *handler = entry->message;
            switch ((signed char)handler->matchKind) {
            case -1: break;
            case 1:
                // The low halfword is a selector; original enum names are unknown.
                switch (handler->matchValue & 0xffffU) {
                case 1:
                    if (receiver == sender) matches = 1;
                    break;
                case 2:
                    if (receiver->nativeObject->scriptObject == sender) matches = 1;
                    break;
                case 3:
                    if (source && source->scriptObject == receiver && receiver != sender) matches = 1;
                    break;
                case 4: {
                    CAITargeting *targeting = &receiver->gameObject->GetSceneNode()->GetAIObject()->targeting;
                    if (sender == targeting->GetTarget().scriptObject ||
                        (targeting->GetTarget().aiObject && sender == targeting->GetTarget().aiObject->scriptObject))
                        matches = 1;
                    break;
                }
                case 5:
                    if (receiver->nativeObject && sender->nativeObject) {
                        int id = receiver->nativeObject->GetID();
                        if (id == sender->nativeObject->GetID() && id) matches = 1;
                    }
                    break;
                }
                break;
            case 0: matches = 1; break;
            case 4:
                switch (handler->matchValue & 0xffffU) {
                case 1:
                    if (receiver == target) matches = 1;
                    break;
                case 2: break;
                case 3:
                    if (target && target->nativeObject->scriptObject == receiver) matches = 1;
                    break;
                }
                break;
            case 2: matches = BSMessageIsGroupMember(source, handler->matchValue); break;
            case 3: matches = BSMessageIsGroupCategoryMember(source, receiver->nativeObject, handler->matchValue); break;
            }
            if (matches) BSMessageQueueMessage(sender, target, source, entry, context);
        }
        entry = BSGetMessageHandlerByIndex(entry->nextIndex);
    }
    return 0;
}
