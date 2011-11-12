/* 
 * File:   Serializable.h
 * Author: Tomáš Čerevka, Adam Činčura
 *
 * Created on November 2, 2011, 10:36 AM
 */

#ifndef SERIALIZABLE_H
#define	SERIALIZABLE_H

/**
 * Abstraktni trida definujici rozhrani pro vsechny objekty, ktere se 
 * budou posilat mezi procesory v ramci komunikace.
 * @author Tomáš Čerevka
 * @author Adam Činčura
 */
class Serializable {
public:
    
    Serializable() {
        
    }
    
    virtual ~Serializable() {
        
    }
    
    /**
     * Serializuje objekt.
     * @param char* Bufferu bytu, do ktereho se serializuje.
     * @param int& Pozice v bufferu.
     */
    virtual void serialize(char*, int&) const = 0;
    
    /**
     * Vytvori objekt ze serializovanych dat.
     * @param char* Bufferu bytu, ze ktereho se deserializuje.
     * @param int& Pozice v bufferu.
     */
    virtual void deserialize(char*, int&) = 0;
    
private:

};

#endif	/* SERIALIZABLE_H */

