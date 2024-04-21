package com.joaob.backendsume.domain.scans;

import jakarta.validation.constraints.NotBlank;

import java.sql.Blob;

public record DadosCadastroScan(
        @NotBlank
        String serial,

        Blob scan_1,
        Blob scan_2,
        Blob scan_3,
        Blob scan_4,
        Blob scan_5
) {
}
