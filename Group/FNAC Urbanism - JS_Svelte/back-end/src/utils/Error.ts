export interface Success {
  success: 'true';
}

export interface IError {
  success: false;
  message: string;
  /**
   * The error message, for developers, only to be used for debugging.
   */
  errorText?: string;
}

export interface BadRequestError extends IError {
  message: 'Bad Request';
  errors: any;
}

export interface InternalError extends IError {
  message: 'Internal Server Error';
}

export interface NotFoundError extends IError {
  message: 'Not Found';
}

export interface UnauthorizedError extends IError {
  message: 'Unauthorized';
}

export interface ForbiddenError extends IError {
  message: 'Forbidden';
}
