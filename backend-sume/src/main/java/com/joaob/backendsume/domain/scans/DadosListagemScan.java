package com.joaob.backendsume.domain.scans;

import java.sql.Blob;

public record DadosListagemScan(String serial, Blob scan_1, Blob scan_2, Blob scan_3, Blob scan_4, Blob scan_5) {

    public DadosListagemScan(Scans scans) {
        this(scans.getSerial(), scans.getScan_1(), scans.getScan_2(), scans.getScan_3(), scans.getScan_4(), scans.getScan_5());
    }
}
