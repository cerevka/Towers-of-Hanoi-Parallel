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
    Serializable();
    Serializable(const Serializable& orig);
    virtual ~Serializable();
    
    /**
     * Vrati serializovany objekt.
     * @return Pole bytu reprezentujici objekt.
     */
    virtual char* serialize(void) = 0;
    
    /**
     * Vrati deserializovany objekt v puvodni podobe.
     * @return Ukazatel na zrekonstruovany objekt.
     */
    virtual Serializable* deserialize(void) = 0;
    
private:

};

#endif	/* SERIALIZABLE_H */

