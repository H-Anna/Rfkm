export interface Etel{
    // id: number;
    // name: string;
    // price: number;


    //ezeket adja vissza a gttp get /etterem/id
    Akcio: {
        Ertek: number,
        Nev: string
    },
    Ar: number,
    Cimke: number[],
    EtelID: number,
    Idoszak: {
        Befejezes: number,
        Kezdes: number
    },
    Kep: any,
    Leiras: string,
    Nev: string
    
}