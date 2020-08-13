;;;;;;;;;;;;;;;;;;;;;
;; web-mode (HTML, CSS, JavaScript)
;;;;;;;;;;;;;;;;;;;;;
(require 'web-mode)
(defun web-mode-hook ()
  (setq web-mode-pre-elements nil)
  (setq web-mode-markup-indent-offset 2)
  (setq web-mode-css-indent-offset 2)
  (setq web-mode-code-indent-offset 2)
  (setq web-mode-comment-style 2)

  (require 'javascript-mode)
  (add-hook 'js-mode-hook
            (lambda ()
              (make-local-variable 'js-indent-level)
              (setq js-indent-level 4)))
  )
