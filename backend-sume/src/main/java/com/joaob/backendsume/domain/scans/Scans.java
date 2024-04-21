package com.joaob.backendsume.domain.scans;

import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;

import java.sql.Blob;

@Table(name = "scans")
@Entity(name = "Scans")
@Getter
@NoArgsConstructor
@AllArgsConstructor
@EqualsAndHashCode(of = "id")
public class Scans {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String serial;
    @Lob
    private Blob scan_1;
    @Lob
    private Blob scan_2;
    @Lob
    private Blob scan_3;
    @Lob
    private Blob scan_4;
    @Lob
    private Blob scan_5;

    public Scans(DadosCadastroScan dados) {
        this.serial = dados.serial();
        this.scan_1 = dados.scan_1();
        this.scan_2 = dados.scan_2();
        this.scan_3 = dados.scan_3();
        this.scan_4 = dados.scan_4();
        this.scan_5 = dados.scan_5();

    }

    public void setScan_1(Blob scan_1) {
        this.scan_1 = scan_1;
    }
    public void setScan_2(Blob scan_2) {
        this.scan_2 = scan_2;
    }
    public void setScan_3(Blob scan_3) {
        this.scan_3 = scan_3;
    }
    public void setScan_4(Blob scan_4) {
        this.scan_4 = scan_4;
    }
    public void setScan_5(Blob scan_5) {
        this.scan_5 = scan_5;
    }

    public void atualizarInformacoes(DadosAtualizacaoScan dados, int escolha) {
        if(dados.scan_1() != null) {
            this.scan_1 = dados.scan_1();
        }
    }

}
