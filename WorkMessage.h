/* 
 * File:   WorkMessage.h
 * Author: Tomáš Čerevka, Adam Činčira
 *
 * Created on November 13, 2011, 11:32 AM
 */

#ifndef WORKMESSAGE_H
#define	WORKMESSAGE_H

#include <vector>

#include "SpaceItem.h"
#include "global.h"


/**
 * Kontejner pro prepreaveni vice {@link SpaceItem}u.
 * @author Tomáš Čerevka
 * @author Adam Činčura
 */
class WorkMessage {
    
public:
    
    /**
     * Konstruktor.
     */
    WorkMessage();
    
    /**
     * Destruktor.
     */
    virtual ~WorkMessage();
    
    /**
     * Prida do kontejneru dalsi {@link SpaceItem}.
     * @param SpaceItem& Pridavany prvek prostoru.
     */
    void addItem(SpaceItem&);
    
    inline const vector<SpaceItem>* getItems(void) const {
        return &items;
    }
    
    /**
     * Serializuje cely kontejner.
     * @param char* Buffer.
     * @param int& Pozice v bufferu.
     */
    void serialize(char*, int&) const;
    
    /**
     * Deserializuje cely kontejner.
     * @paream char* Buffer.
     * @param int& Pozice v bufferu.
     */
    void deserialize(char*, int&);
    
    /**
     * Pretizeny operator pro vypis kontejneru.
     * @param ostream& Vstupni stream.
     * @param const WorkMessage& Vypisovany kontejner.
     * @return ostream& Vystupni stream.
     */
    friend ostream& operator<<(ostream&, const WorkMessage&);
    
private:
    
    vector<SpaceItem> items;

};

#endif	/* WORKMESSAGE_H */

