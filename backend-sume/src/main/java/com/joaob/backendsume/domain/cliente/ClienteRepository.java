package com.joaob.backendsume.domain.cliente;

import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

public interface ClienteRepository extends JpaRepository<Cliente, Long> {

    Page<Cliente> findAllByAtivoTrue(Pageable paginacao);

    @Query("""
            select p.ativo
            from Cliente p
            where
            p.cpf_cnpj = :cpf_cnpj
            """)
    Boolean findAtivoByCpf(String cpf);
}
