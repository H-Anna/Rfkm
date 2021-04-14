import { Nap } from 'src/app/ettermek/model/nap';
export interface Etterem {
    Cim: {
        Emelet_ajto: string,
        Hazszam: string,
        Irsz: number,
        Kozterulet: string
    },
    Cimke: string[],
    EtteremID: number,
    Kep: "����",
    Leiras: string,
    Nev: string,
    Nyitvatartas: Nap[],       
    Szallitasi_ktsg: number
}

