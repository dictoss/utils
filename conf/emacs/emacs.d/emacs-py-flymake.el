;;;;;;;;;;;;;;;;;;;;;;;;
;; python
;;;;;;;;;;;;;;;;;;;;;;;;

;; flymake for python
;;
;; your install.
;; # pip install pep8 flymake pychecker
;;

(require 'flymake)

(when (load "flymake" t)
  (defun flymake-pep8-init ()
    (let* ((temp-file (flymake-init-create-temp-buffer-copy
                       'flymake-create-temp-inplace))
           (local-file (file-relative-name
                        temp-file
                        (file-name-directory buffer-file-name)))) 
      ;; (list "pychecker" (list local-file))))
      (list "pep8" (list local-file)))
))

(load "~/.emacs.d/flymake-cursor.el")
(add-to-list 'flymake-allowed-file-name-masks
             '("\\.py\\'" flymake-pep8-init))
(add-hook 'python-mode-hook
          '(lambda ()
             (flymake-mode t)))

(global-set-key [f7] 'flymake-goto-prev-error)
(global-set-key [f8] 'flymake-goto-next-error)

