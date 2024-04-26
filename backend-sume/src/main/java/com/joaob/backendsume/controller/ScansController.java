package com.joaob.backendsume.controller;

import com.joaob.backendsume.domain.scans.*;
import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.web.PageableDefault;
import org.springframework.http.ResponseEntity;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.util.UriComponentsBuilder;

import java.io.IOException;
import java.sql.Blob;
import java.sql.SQLException;
import java.util.List;
import java.util.Optional;

public class ScansController {

    @Autowired
    private ScanRepository repository;

    private boolean temCampoVazio(Scans scan) {
        try {
            return isEmptyBlob(scan.getScan_1()) ||
                   isEmptyBlob(scan.getScan_2()) ||
                   isEmptyBlob(scan.getScan_3()) ||
                   isEmptyBlob(scan.getScan_4()) ||
                   isEmptyBlob(scan.getScan_5());
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    private Blob converteArquivoBlob(MultipartFile file) throws IOException, SQLException {
        return new javax.sql.rowset.serial.SerialBlob(file.getBytes());
    }

    @PostMapping
    @Transactional
    public ResponseEntity cadastrar(MultipartFile file, UriComponentsBuilder uribuilder) throws IOException,SQLException {
        try {
            List<Scans> scans = repository.findAll();

            if (!scans.isEmpty()) {
                Optional<Scans> scanCampoVazio = scans.stream()
                        .filter(this::temCampoVazio)
                        .findFirst();

                if (scanCampoVazio.isPresent()) {
                    Scans scan = scanCampoVazio.get();
                    if (isEmptyBlob(scan.getScan_1())) {
                        scan.setScan_1(converteArquivoBlob(file));
                    } else if (isEmptyBlob(scan.getScan_2())) {
                        scan.setScan_2(converteArquivoBlob(file));
                    } else if (isEmptyBlob(scan.getScan_3())) {
                        scan.setScan_3(converteArquivoBlob(file));
                    } else if (isEmptyBlob(scan.getScan_4())) {
                        scan.setScan_4(converteArquivoBlob(file));
                    } else if (isEmptyBlob(scan.getScan_5())) {
                        scan.setScan_5(converteArquivoBlob(file));
                    }
                    repository.save(scan);
                    var uri = uribuilder.path("/scans/{serial}").buildAndExpand(scan.getSerial()).toUri();
                    return ResponseEntity.created(uri).body(new DadosDetalhamentoScan(scan));

                } else {
                    Scans scan = new Scans();
                    scan.setScan_1(converteArquivoBlob(file));
                    repository.save(scan);
                    var uri = uribuilder.path("/scans/{serial}").buildAndExpand(scan.getSerial()).toUri();
                    return ResponseEntity.created(uri).body(new DadosDetalhamentoScan(scan));

                }

            } else {
                Scans scan = new Scans();
                scan.setScan_1(converteArquivoBlob(file));
                repository.save(scan);
                var uri = uribuilder.path("/scans/{serial}").buildAndExpand(scan.getSerial()).toUri();
                return ResponseEntity.created(uri).body(new DadosDetalhamentoScan(scan));
            }
        } catch (IOException | SQLException e) {
            return ResponseEntity.internalServerError().body("Erro ao processar o mapeamento: " +e.getMessage());
        }

    }

    @GetMapping
    public ResponseEntity <Page<DadosListagemScan>> listar(@PageableDefault(size = 5, sort = {"serial"}) Pageable paginacao) {
        Scans scan = new Scans();
        var dados = scan.getSerial();

        var page = repository.findBySerial(dados, paginacao).map(DadosListagemScan::new);
        return ResponseEntity.ok(page);
    }

    private boolean isEmptyBlob(Blob blob) throws SQLException {
        return blob == null || blob.length() == 0;
    }

    @DeleteMapping("/{serial}")
    @Transactional
    public ResponseEntity excluir(@PathVariable String serial) {
        repository.deleteBySerial(serial);
        return ResponseEntity.noContent().build();
    }
}
