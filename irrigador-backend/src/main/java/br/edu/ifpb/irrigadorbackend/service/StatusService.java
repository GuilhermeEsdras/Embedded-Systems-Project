package br.edu.ifpb.irrigadorbackend.service;

import br.edu.ifpb.irrigadorbackend.entity.Status;
import br.edu.ifpb.irrigadorbackend.repository.StatusRepository;
import lombok.RequiredArgsConstructor;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;

@Service
@RequiredArgsConstructor
public class StatusService {

    private final StatusRepository statusRepository;

    public Status add(Status status) {
        return statusRepository.save(status);
    }

    public Page<Status> findPage(Pageable pageable) {
        return statusRepository.findAll(pageable);
    }
}
