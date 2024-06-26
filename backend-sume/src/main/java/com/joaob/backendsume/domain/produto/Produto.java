package com.joaob.backendsume.domain.produto;


import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;

@Table(name = "produto")
@Entity(name = "Produto")
@Getter
@NoArgsConstructor
@AllArgsConstructor
@EqualsAndHashCode(of = "id")
public class Produto {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String serial;
    private String modelo;
    private Boolean ativo;

    public Produto(DadosCadastroProduto dados) {
        this.ativo = true;
        this.serial = dados.serial();
        this.modelo = dados.modelo();
    }

    public void excluir() {
        this.ativo = false;
    }

}
