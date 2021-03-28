import { Nap } from "src/app/etterem-regisztralasa/model/nap";

export class Regisztracio{
    email: string;
    jelszo: string;
    nev: string;
    leiras: string;
    cim: {
        irsz: number,
		kozterulet: string,
		hazszam: number,
		emelet_ajto: string;
    }
    nyitvatartas: Nap[] = [];

    cimke: string;
    //kep: kep xd;
    irszamok: number[];
    szallKtg: number;
}