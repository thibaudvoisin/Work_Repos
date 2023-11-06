import {
  type IError,
  type NotFoundError,
  type BadRequestError,
  type InternalError,
  type UnauthorizedError,
  type ForbiddenError,
} from './Error';

/*
Here is the syntax and an exemple to throw errors

  Syntax:
    throw ApiError.[Name of the error]('comment')

  Exemple:
    throw ApiError.badRequest('Missing parameter')
*/

export class ApiError<Err extends IError> extends Error {
  constructor(
    private readonly code: number,
    private readonly err: Err
  ) {
    super(err.message);
  }

  public get error(): Err {
    return this.err;
  }

  public get statusCode(): number {
    return this.code;
  }

  public static notFound(errorText: string): ApiError<NotFoundError> {
    return new ApiError(404, {
      success: false,
      message: 'Not Found',
      errorText,
    });
  }

  public static badRequest(err: any): ApiError<BadRequestError> {
    return new ApiError(400, {
      success: false,
      message: 'Bad Request',
      errors: err,
    });
  }

  public static internal(errorText: string): ApiError<InternalError> {
    return new ApiError(500, {
      success: false,
      message: 'Internal Server Error',
      errorText,
    });
  }

  public static unauthorized(errorText?: string): ApiError<UnauthorizedError> {
    return new ApiError(401, {
      success: false,
      message: 'Unauthorized',
      errorText,
    });
  }

  public static forbidden(errorText?: string): ApiError<ForbiddenError> {
    return new ApiError(403, {
      success: false,
      message: 'Forbidden',
      errorText,
    });
  }
}
